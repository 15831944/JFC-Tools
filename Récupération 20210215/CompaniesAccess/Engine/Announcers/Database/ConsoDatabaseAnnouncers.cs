using AR7Common;
using Database;
using RadioEngine;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;

namespace CompaniesAccess
{
    /// <summary>
    /// the announcers from the Consolidation database
    /// </summary>
    public class ConsoDatabaseAnnouncers : IAnnouncerDataSource
    {
        #region static members

        private static String s_consoDatabaseFilename = "KeyConso.db";

        private static int s_announcerBranch = 3;
        private static int s_announcerDepth = 2;

        private static Int64 s_minVersion = 15;

        #endregion

        #region private members

        private IDatabaseManager m_databaseManager;
        private IHelperDataBase m_helper;

        /// <summary>
        /// the available announcers
        /// </summary>
        private IEnumerable<String> m_announcers;

        /// <summary>
        /// the announcers rights for all the users
        /// </summary>
        /// <remarks>
        /// key : user name
        /// value : list of announcer names
        /// </remarks>
        private Dictionary<String, Dictionary<String, bool>> m_announcerRights;

        #endregion

        #region IAnnouncerDataSource implementation

        public void Initialize()
        {
            // look for the conso data path in the configuration
            if (ConfigurationManager.Singleton.Config.Paths.ContainsKey(RuntimeInfoManager.KeysLabels[44]))
            {
                String l_dataPath = Path.GetFullPath(ConfigurationManager.Singleton.Config.Paths[RuntimeInfoManager.KeysLabels[44]]);
                if (!String.IsNullOrEmpty(l_dataPath))
                {
                    // for a UNC path, the connection path need 2 other "\\"
                    String l_path = Path.Combine(this.ConvertUNCPath(l_dataPath), s_consoDatabaseFilename);
                    if (File.Exists(l_path))
                    {
                        m_helper = new ConsoDataBaseHelper();

                        m_helper.PathConnection = l_path;
                        m_helper.PasswordConnection = String.Empty;

                        this.m_databaseManager = new DatabaseSQLite(m_helper);

                        // check the database version
                        if (this.IsValidVersion())
                            this.LoadAnnouncerRights();
                        else
                        {
                            this.m_databaseManager = null;
                            this.m_helper = null;
                        }
                    }
                    else
                        ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MAJOR, String.Format("Le fichier \"{0}\" est introuvable. Veuillez contacter votre fournisseur.", l_path));
                }
            }
            else
                ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MAJOR, "Le chemin du fichier de base de données n'existe pas dans la configuration. Veuillez contacter votre fournisseur.");
        }

        private bool IsValidVersion()
        {
            bool l_valid = false;

            if (this.m_databaseManager != null)
            {
                this.m_databaseManager.Open();

                try
                {
                    DataTable l_table = this.m_databaseManager.GetDataTable(String.Format("SELECT Reference FROM {0}", m_helper.DataBaseInfos.GetTableName((short)TableName.TableName_Version)));
                    if (l_table != null && l_table.Rows.Count == 1)
                    {
                        Int64 l_version = (Int64)l_table.Rows[0][0];
                        l_valid = (l_version >= s_minVersion);
                        if (!l_valid)
                            ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MAJOR, "Version de la base de données invalide. Veuiilez contacter votre fournisseur.");
                    }

                    if (l_valid &&
                        !this.m_databaseManager.ExistTable(
                            m_helper.DataBaseInfos.GetTableName((short) TableName.TableName_AnnouncerRights)))
                    {
                        l_valid = false;
                        ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MAJOR, String.Format("La table des droits d'accès des annonceurs n'existe pas. Veuillez contacter votre fournisseur."));
                    }
                }
                catch (Exception l_ex)
                {
                    ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MINOR, String.Format("Database version query error : {0}" + l_ex.Message));
                }
                finally
                {
                    this.m_databaseManager.Close();
                }
            }

            return l_valid;
        }

        private void LoadAnnouncerRights()
        {
            this.m_announcerRights = new Dictionary<string, Dictionary<string, bool>>();

            if (this.m_databaseManager != null)
            {
                this.m_databaseManager.Open();

                try
                {
                    // gets all the announcer rights from the database
                    DataTable l_table = this.m_databaseManager.GetDataTable(String.Format("SELECT Utilisateur, AnnonceurJFC FROM {0}", m_helper.DataBaseInfos.GetTableName((short)TableName.TableName_AnnouncerRights)));
                    if (l_table != null)
                    {
                        // fills the announcers rights
                        foreach (DataRow l_dataRow in l_table.Rows)
                        {
                            String l_userName = (String)l_dataRow[0];
                            String l_announcerName = (String)l_dataRow[1];
                            if (!this.m_announcerRights.ContainsKey(l_userName))
                                this.m_announcerRights.Add(l_userName, new Dictionary<String, bool>());
                            this.m_announcerRights[l_userName].Add(l_announcerName, true);
                        }
                    }
                }
                catch (Exception l_ex)
                {
                    ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MINOR, String.Format("Database announcers rights query error : {0}" + l_ex.Message));
                }
                finally
                {
                    this.m_databaseManager.Close();
                }
            }
        }

        public void Open()
        {
            this.m_databaseManager.Open();
        }

        public void Close()
        {
            this.m_databaseManager.Close();
        }

        public IEnumerable<String> Announcers
        {
            get
            {
                if (this.m_announcers == null && this.m_databaseManager != null)
                {
                    this.m_databaseManager.Open();

                    try
                    {
                        DataTable l_table = this.m_databaseManager.GetDataTable(String.Format("SELECT Libelle FROM {0} WHERE Branch={1} AND Profondeur={2}", m_helper.DataBaseInfos.GetTableName((short)TableName.TableName_ClassificationBody), s_announcerBranch, s_announcerDepth));
                        List<String> l_dataAnnouncers = new List<String>();
                        foreach (DataRow l_dataRow in l_table.Rows)
                            l_dataAnnouncers.Add((String)l_dataRow[0]);

                        this.m_announcers = l_dataAnnouncers
                            .Distinct()
                            .OrderBy(a => a)
                            .ToList();
                    }
                    catch (Exception l_ex)
                    {
                        ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MINOR, String.Format("Database classification query error : {0}" + l_ex.Message));
                    }
                    finally
                    {
                        this.m_databaseManager.Close();
                    }
                }

                return this.m_announcers;
            }
        }

        public bool AnnouncerIsValid(String _userName, String _announcerName)
        {
            return (this.m_announcerRights != null && this.m_announcerRights.ContainsKey(_userName) && this.m_announcerRights[_userName].ContainsKey(_announcerName));
        }

        public void UpdateRights(List<Tuple<String, String>> _rights)
        {
            if (_rights != null && this.m_databaseManager != null)
            {
                this.m_databaseManager.Open();

                try
                {
                    this.m_databaseManager.BeginTransaction();

                    // in a first time, we need to delete all the rights
                    this.m_databaseManager.Delete("DroitsAnnonceurs", null);

                    // then we can add the rights
                    Dictionary<String, String> l_data;
                    foreach (Tuple<String, String> l_right in _rights)
                    {
                        l_data = new Dictionary<String, String>();
                        l_data["Utilisateur"] = l_right.Item1;
                        l_data["AnnonceurJFC"] = l_right.Item2;
                        this.m_databaseManager.Insert("DroitsAnnonceurs", l_data);
                    }
                }
                catch (Exception l_ex)
                {
                    ErrorManagement.Singleton.Error(ErrorManagement.TypeError.MINOR, String.Format("Database announcers query error : {0}" + l_ex.Message));
                }
                finally
                {
                    this.m_databaseManager.CommitTransaction();
                    this.m_databaseManager.Close();
                }
            }
        }

        #endregion

        #region tools

        /// <summary>
        /// for a UNC path, the connection path need 2 other "\\"
        /// </summary>
        /// <param name="_path"></param>
        /// <returns></returns>
        private String ConvertUNCPath(String _path)
        {
            String l_newPath = _path;
            if (!String.IsNullOrEmpty(_path) && _path.StartsWith("\\\\"))
                l_newPath = String.Format("\\\\{0}", _path);
            return l_newPath;
        }

        #endregion
    }
}

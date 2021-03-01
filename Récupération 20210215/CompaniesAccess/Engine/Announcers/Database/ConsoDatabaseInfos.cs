using System;
using Database;
using System.Collections.Generic;

namespace CompaniesAccess
{
    internal enum TableName : short
    {
        TableName_Version = 0,
        TableName_AnnouncerRights,
        TableName_ClassificationBody
    }

    public class ConsoDatabaseInfos : ADataBaseInfos
    {
        public ConsoDatabaseInfos()
        {
            m_TableNames.Add((short)TableName.TableName_Version, "Version");
            m_TableNames.Add((short)TableName.TableName_AnnouncerRights, "DroitsAnnonceurs");
            m_TableNames.Add((short)TableName.TableName_ClassificationBody, "ClassifBody");

            m_columns.Add((short)TableName.TableName_Version, VersionColumns);
            m_columns.Add((short)TableName.TableName_AnnouncerRights, AnnouncerRightsColumns);
            m_columns.Add((short)TableName.TableName_ClassificationBody, ClassificationBodyColumns);
        }

        public DatabaseColumn[] VersionColumns
        {
            get
            {
                List<DatabaseColumn> _columns = new List<DatabaseColumn>();
                _columns.Add(new DatabaseColumn("Reference", m_typeIntegerPrimaryKeyAuto));
                return _columns.ToArray();
            }
        }

        public DatabaseColumn[] AnnouncerRightsColumns
        {
            get
            {
                List<DatabaseColumn> _columns = new List<DatabaseColumn>();
                _columns.Add(new DatabaseColumn("Reference", m_typeIntegerPrimaryKeyAuto));
                _columns.Add(new DatabaseColumn("AnnonceurJFC", m_typeText128));
                _columns.Add(new DatabaseColumn("Utilisateur", m_typeText128));
                return _columns.ToArray();
            }
        }

        public DatabaseColumn[] ClassificationBodyColumns
        {
            get
            {
                List<DatabaseColumn> _columns = new List<DatabaseColumn>();
                _columns.Add(new DatabaseColumn("Reference", m_typeIntegerPrimaryKeyAuto));
                _columns.Add(new DatabaseColumn("Branch", m_typeInteger));
                _columns.Add(new DatabaseColumn("Profondeur", m_typeInteger));
                _columns.Add(new DatabaseColumn("Libelle", m_typeText255));
                _columns.Add(new DatabaseColumn("Dispositif", m_typeInteger));
                return _columns.ToArray();
            }
        }

    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.OracleClient;
using System.Data;
using System.Data.OleDb;
using System.IO;
using AtOracle_NRJ;
using System.Collections;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Threading;




namespace AtOracle_NRJ
{


    public class ConnecOracle
    {

        private int iChoixProcSto;
        private string strNomFichier;
        private string strNomFichierSecteur;
        private string strNomFichierAnnonceur;
        private string strStation;
        private string strRegion;
        private string strDtFromDate;
        private string strDtToDate;
        private DateTime dtFromDate;
        private DateTime dtToDate;
        private int iNumAnnonceur;
        private string listSecteur;
        private string _oraDB;
        private string strSqlArgs;


        /// <summary>
        /// constructeur de la connection a la base NRJ avec choix de la requete
        /// </summary>
        public ConnecOracle(string sqlChoice)//string oradb,
        {
            try
            {
                //oraDB = oradb;
                strSqlArgs = sqlChoice;
                String[] strChaine;
                strChaine = sqlChoice.Split(';');
                int longArrayChaine = sqlChoice.Split(';').Length;
                //pour sortir un ou deux fichiers
                int longArrayProcStock = strChaine[0].Split(',').Length;

                if (longArrayProcStock == 1)
                    iChoixProcSto = Convert.ToInt32(strChaine[0]);
                else
                    iChoixProcSto = 4;


                //procedure avec passage de parametres
                if (strChaine[0] == "3")
                {
                    strStation = strChaine[1];
                    strRegion = strChaine[2];
                    strDtFromDate = strChaine[3];
                    dtFromDate = Convert.ToDateTime(strChaine[3]);
                    strDtToDate = strChaine[4];
                    dtToDate = Convert.ToDateTime(strChaine[4]);
                    strNomFichier = strChaine[5];
                    if (longArrayChaine == 7)
                    {
                        iNumAnnonceur = Convert.ToInt32(strChaine[6]);
                        listSecteur = "";
                    }

                    if (longArrayChaine == 8)
                    {
                        if (strChaine[6].ToString() == "")
                            iNumAnnonceur = 0;
                        else
                            iNumAnnonceur = Convert.ToInt32(strChaine[6]);

                        listSecteur = strChaine[7].Replace(",", " ");
                    }

                }

                if (iChoixProcSto == 1 || iChoixProcSto == 2 || iChoixProcSto == 5)
                {
                    strNomFichier = strChaine[1];
                }

                if (iChoixProcSto == 4)
                {
                    strNomFichierSecteur = strChaine[1] + "\\NRJ_SECTEUR.CSV ";
                    strNomFichierAnnonceur = strChaine[1] + "\\NRJ_ANNONCEUR.CSV ";
                }


            }
            catch (System.Exception ex)
            {
                MessageBox.Show(ex.Message + " " + strSqlArgs, "Probleme de connexion oracle avec les arguments passes");
                return;
                //Application.Exit();
            }


            try
            { 
                DataView dv = RequestOracle();
                if (iChoixProcSto != 5)
                {
                    if (iChoixProcSto != 4)
                        CreateCSVFile(dv, strNomFichier + ".csv"); //Environment.CurrentDirectory + "\\" + 
                }
            }
            catch (System.Exception ex2)
            {
                Application.Exit();
            }           


        }


        //Méthode appelant la fonction GetPrivateProfileString
        //Retourne la valeur de la clé spécifiée
        public string RecupererValeur(string section, string cle)
        {
            //Chaine de caractère récupérant la valeur de la clé
            StringBuilder temp_ini = new StringBuilder(255);
            string path = Environment.CurrentDirectory + "\\ATAAETUD.INI";


            if (File.Exists("Ataaetud.ini"))
            {
                GetPrivateProfileString(section, cle, "", temp_ini, 255, path);//@"C:\Users\Jean-Marie.Nivot\Documents\Visual Studio 2008\Projects\OracleTNS_NRJ\OracleTNS_NRJ\bin\Release\ATAAETUD.INI"
                return temp_ini.ToString();
            }
            else
                return "";
        }

        [DllImport("kernel32.dll")]
        static extern uint GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize, string lpFileName);


        public DataView RequestOracle()
        {
            string ping = RecupererValeur("Definition_du_site", "Ping_Regie");
            string port = RecupererValeur("Definition_du_site", "Port_Regie");
            string userid = RecupererValeur("Definition_du_site", "UserId");
            string password = RecupererValeur("Definition_du_site", "Password");
            string Bdd = RecupererValeur("Definition_du_site", "Service_Name");

            string oradb = "Data Source= (DESCRIPTION="
             + "(ADDRESS =(PROTOCOL=TCP)(HOST=" + ping + " )(PORT=" + port + "))"
             + "(CONNECT_DATA=(SERVER=DEDICATED)(SERVICE_NAME=" + Bdd + ")));"
             + "User Id=" + userid + ";Password=" + password + ";";


            _oraDB = oradb;

            OracleConnection conn = new OracleConnection(oradb);

            try
            {
                conn.Open();
            }
            catch (System.Exception ex)
            {
                String error = "Pas de connexion chez NRJ" + ex.Message;
                MessageBox.Show(error + "chaine : " + oradb, "Probleme de connexion avec la base de données");
                throw ex;
                
                //Application.Exit();
            }

            DataSet ds = new DataSet();
            DataTable dt = new DataTable();
            String titleProcStock = "";
            String oraParam = "";

            switch (iChoixProcSto)
            {
                case 1:
                    titleProcStock = "NELIS.P_JFC_GET_LIST_COMPETITION";
                    oraParam = "competition_cv";
                    break;
                case 2:
                    titleProcStock = "NELIS.P_JFC_GET_LIST_ADVERTISER";
                    oraParam = "advertiser_cv";
                    break;
                case 3:
                    titleProcStock = "NELIS.P_JFC_Get_List_Blocks";
                    oraParam = "block_cv";
                    break;
                case 4:
                    GenererDeuxFichiers(conn);
                    return null;
                    break;
                case 5:
                    lectureEcritureFichierEcranDispo(strNomFichier, conn);
                    return null;

            }

            OracleCommand myCMD;
            myCMD = new OracleCommand();
            myCMD.Connection = conn;
            myCMD.CommandText = titleProcStock;
            myCMD.CommandType = CommandType.StoredProcedure;


            // choix == 3 passage de parametres TODO Attention / annonceur ou pas, secteur ou pas
            if (iChoixProcSto == 3)
            {
                myCMD.Parameters.Add("a_station", OracleType.VarChar, 2).Value = strStation;
                myCMD.Parameters.Add("a_region", OracleType.VarChar, 2).Value = strRegion;
                myCMD.Parameters.Add("a_from_date", OracleType.DateTime, 10).Value = dtFromDate;
                myCMD.Parameters.Add("a_to_date", OracleType.DateTime, 10).Value = dtToDate;
                if (listSecteur == "" || listSecteur == null)
                    myCMD.Parameters.Add("as_sid_competition", OracleType.VarChar, 10).Value = DBNull.Value;
                else
                    myCMD.Parameters.Add("as_sid_competition", OracleType.VarChar, 10).Value = listSecteur;
                if (iNumAnnonceur == 0)
                    myCMD.Parameters.Add("n_nid_company", OracleType.Int16, 10).Value = DBNull.Value;
                else
                    myCMD.Parameters.Add("n_nid_company", OracleType.Int16, 10).Value = iNumAnnonceur;

                myCMD.Parameters.Add(new OracleParameter(oraParam, OracleType.Cursor)).Direction = ParameterDirection.Output;
            }
            else
            {
                myCMD.Parameters.Add(new OracleParameter(oraParam, OracleType.Cursor)).Direction = ParameterDirection.Output;
            }

            OracleDataReader myReader;
            try
            {
                myCMD.ExecuteNonQuery();
            }
            catch (System.Exception ex)
            {
                string mess = ex.Message;
                MessageBox.Show(mess + " " + strSqlArgs, "Probleme de connexion avec executeNonQuery");
                throw ex;
                //Application.Exit();
            }

            myReader = myCMD.ExecuteReader();


            // Initialisation du tableau de retour
            String[,] sTabResult = null;

            int m_iNombreLignes = 0;
            int m_iNombreColonnes = myReader.FieldCount;
            while (myReader.Read())
                m_iNombreLignes++;

            sTabResult = new String[m_iNombreLignes, m_iNombreColonnes];

            myReader.Close();
            myReader = myCMD.ExecuteReader();

            int i = 0;

            // pour le procedure stockee 3 : liste des ecrans
            if (iChoixProcSto == 3)
            {
                while (myReader.Read())
                {
                    for (int j = 0; j < m_iNombreColonnes; j++)
                        if (myReader.IsDBNull(j) == false)
                        {
                            switch (j)
                            {
                                case 0:
                                    sTabResult[i, j] = FormatHeure(myReader.GetOracleValue(j).ToString());
                                    break;
                                case 1:
                                    sTabResult[i, j] = (Convert.ToInt32(myReader.GetOracleValue(j).ToString()) - 1).ToString();
                                    break;
                                case 2:
                                    sTabResult[i, j] = FormatDate(myReader.GetOracleValue(j).ToString());
                                    break;
                                case 3:
                                    sTabResult[i, j] = FormatHeure(myReader.GetOracleValue(j).ToString());
                                    break;
                                default:
                                    sTabResult[i, j] = myReader.GetOracleValue(j).ToString();
                                    break;
                            }
                        }
                        else
                        {
                            sTabResult[i, j] = ""; // Met "" dans les champs null
                        }
                    i++;
                }
            }

            // pour le procedure stockee 1 ou 2 : liste des secteurs et liste des annonceurs
            if (iChoixProcSto == 1 || iChoixProcSto == 2)
            {
                while (myReader.Read())
                {
                    for (int j = 0; j < m_iNombreColonnes; j++)
                        if (myReader.IsDBNull(j) == false)
                        {
                            switch (j)
                            {
                                case 0:
                                    //if (iChoixProcSto == 1)
                                    //    sTabResult[i, j] = FormatInteg(myReader.GetOracleValue(j).ToString());
                                    //else
                                    sTabResult[i, j] = myReader.GetOracleValue(j).ToString();
                                    break;
                                case 1:
                                    sTabResult[i, j] = myReader.GetOracleValue(j).ToString();
                                    break;
                                case 2:
                                    sTabResult[i, j] = (Convert.ToInt32(myReader.GetOracleValue(j).ToString()) - 1).ToString();
                                    break;
                                default:
                                    sTabResult[i, j] = myReader.GetOracleValue(j).ToString();
                                    break;
                            }
                        }
                        else
                        {
                            sTabResult[i, j] = ""; // Met "" dans les champs null
                        }
                    i++;
                }
            }

            //////////////////////////////////////////////////////////////////

            myReader.Close();
            conn.Close();

            dt = new DataTable();
            DataView dv = dt.DefaultView;

            /////////////////////////////////////////////////////////////////

            // choix == 3
            if (iChoixProcSto == 3)
            {
                //DataColumn dc0;
                //dc0 = new DataColumn(strStation.ToString(), Type.GetType("System.String"));//"Code"
                //dt.Columns.Add(dc0);
                //dc0 = new DataColumn(strRegion.ToString(), Type.GetType("System.String"));//Int32
                //dt.Columns.Add(dc0);
                //dc0 = new DataColumn(strDtFromDate.ToString(), Type.GetType("System.String"));//Int32
                //dt.Columns.Add(dc0);
                //dc0 = new DataColumn(strDtToDate.ToString(), Type.GetType("System.String"));
                //dt.Columns.Add(dc0);
                ////dc0 = new DataColumn("0", Type.GetType("System.String"));
                ////dt.Columns.Add(dc0);
                ////dc0 = new DataColumn("1", Type.GetType("System.String"));
                ////dt.Columns.Add(dc0);
                ////dc0 = new DataColumn("3", Type.GetType("System.String"));
                ////dt.Columns.Add(dc0);
                //DataRow newRow0 = dt.NewRow();
                //newRow0[0] = "";
                ////newRow0[1] = strRegion;
                ////newRow0[2] = strDtFromDate;
                ////newRow0[3] = strDtToDate;
                //dt.Rows.Add(newRow0);



                DataColumn dc;
                dc = new DataColumn("Code", Type.GetType("System.String"));//"Code"
                dt.Columns.Add(dc);
                dc = new DataColumn("NJour", Type.GetType("System.String"));//Int32
                dt.Columns.Add(dc);
                dc = new DataColumn("NbdeJourDebut", Type.GetType("System.String"));//Int32
                dt.Columns.Add(dc);
                dc = new DataColumn("Heure diffusion", Type.GetType("System.String"));
                dt.Columns.Add(dc);
                dc = new DataColumn("DureeTotale", Type.GetType("System.String"));
                dt.Columns.Add(dc);
                dc = new DataColumn("NJDuree", Type.GetType("System.String"));
                dt.Columns.Add(dc);
                dc = new DataColumn("Concurrence", Type.GetType("System.String"));
                dt.Columns.Add(dc);

                //DataRow newRow0 = dt.NewRow();
                //newRow0[0] = strStation;
                //newRow0[1] = strRegion;
                //newRow0[2] = strDtFromDate;
                //newRow0[3] = strDtToDate;
                //dt.Rows.Add(newRow0);


                for (int z = 1; z < i; z++)
                {
                    DataRow newRow = dt.NewRow();

                    for (int col = 0; col < m_iNombreColonnes; col++)
                    {
                        newRow[col] = sTabResult[z, col];
                    }

                    dt.Rows.Add(newRow);
                }

                dv.Sort = "NJour ASC, NbdeJourDebut ASC, Code ASC";

            }

            // choix == 1
            if (iChoixProcSto == 1)
            {
                DataColumn dc;
                dc = new DataColumn("Code Secteur", Type.GetType("System.String"));
                dt.Columns.Add(dc);
                dc = new DataColumn("Dimension", Type.GetType("System.String"));
                dt.Columns.Add(dc);
                dc = new DataColumn("Niveau", Type.GetType("System.Int32"));
                dt.Columns.Add(dc);


                for (int z = 0; z < i; z++)
                {
                    DataRow newRow = dt.NewRow();

                    for (int col = 0; col < m_iNombreColonnes; col++)
                    {
                        newRow[col] = sTabResult[z, col];
                    }

                    dt.Rows.Add(newRow);
                }

                dv.Sort = "Code Secteur ASC";
            }

            // choix == 2
            if (iChoixProcSto == 2)
            {
                DataColumn dc;
                dc = new DataColumn("Numero Annonceur", Type.GetType("System.Int32"));
                dt.Columns.Add(dc);
                dc = new DataColumn("Nom Annonceur", Type.GetType("System.String"));
                dt.Columns.Add(dc);

                for (int z = 0; z < i; z++)
                {
                    DataRow newRow = dt.NewRow();

                    for (int col = 0; col < m_iNombreColonnes; col++)
                    {
                        newRow[col] = sTabResult[z, col];
                    }

                    dt.Rows.Add(newRow);
                }
            }

            return dv;

        }


        //ajoute 2 zero a la fin et converti en int pour avoir un int de "6 caracteres"
        private String FormatInteg(String codeSecteur)
        {
            if (codeSecteur.Length < 6)
                codeSecteur = codeSecteur + "00";

            return codeSecteur;

        }

        // ajoute 24 heure a l'heure si celle ci est inferieur a 5H00
        private string FormatHeure(string p)
        {

            int lennn = p.Length;
            string ret = "";

            if (Convert.ToInt32(p.Substring(0, 2)) < 5)
                ret = ((24 + Convert.ToInt32(p.Substring(0, 2))).ToString() + p.Substring(2, 3)).ToString().Substring(0, 5);
            else
                ret = p.Substring(0, 5);

            return ret;
        }

        // transforme la date de diffusion du spot en nombre de jour depuis la date début
        private string FormatDate(string p)
        {
            TimeSpan delta = Convert.ToDateTime(p) - dtFromDate;
            return delta.TotalDays.ToString();
        }

        private void CreateBinaryFile(DataTable dt, string strFilePath)
        {

            //FileStream fs = new FileStream(strFilePath, FileMode.OpenOrCreate, FileAccess.Write);
            //BinaryWriter sw = new BinaryWriter(fs);

            //int iColCount = dt.Columns.Count;

            ////for (int i = 0; i < iColCount; i++)
            ////   {
            ////        sw.Write(dt.Columns[i].ToString());
            ////   }

            //// Now write all the rows.

            //foreach (DataRow dr in dt.Rows)
            //{
            //    for (int i = 0; i < iColCount; i++)
            //    {
            //        if (!Convert.IsDBNull(dr[i]))
            //        {
            //           //fs.Seek((long)i, SeekOrigin.Begin);
            //           sw.Write((BitConverter.GetBytes(dr[i].ToString()));
            //        }
            //    }
            //}
            //sw.Close();
            //fs.Close();
        }

        private void GenererDeuxFichiers(OracleConnection connec)
        {
            DataSet ds = new DataSet();
            DataTable dt = new DataTable();
            String titleProcStock = "";
            String oraParam = "";
            OracleDataReader myReader;
            // iteration 1 et 2 corespondants au secteur et annonceur
            for (int z = 1; z < 3; z++)
            {
                switch (z)
                {
                    case 1:
                        titleProcStock = "NELIS.P_JFC_GET_LIST_COMPETITION";
                        oraParam = "competition_cv";
                        break;
                    case 2:
                        titleProcStock = "NELIS.P_JFC_GET_LIST_ADVERTISER";
                        oraParam = "advertiser_cv";
                        break;
                }


                OracleCommand myCMD;
                myCMD = new OracleCommand();
                myCMD.Connection = connec;
                myCMD.CommandText = titleProcStock;
                myCMD.CommandType = CommandType.StoredProcedure;


                myCMD.Parameters.Add(new OracleParameter(oraParam, OracleType.Cursor)).Direction = ParameterDirection.Output;

                try
                {
                    myCMD.ExecuteNonQuery();
                }
                catch (System.Exception ex)
                {
                    string mess = ex.Message;
                    MessageBox.Show(mess + " avec Arguments : " + strSqlArgs, "Probleme de connexion");
                    return;
                    //Application.Exit();
                }

                myReader = myCMD.ExecuteReader();


                // Initialisation du tableau de retour
                String[,] sTabResult = null;


                int m_iNombreLignes = 0;
                int m_iNombreColonnes = myReader.FieldCount;
                while (myReader.Read())
                    m_iNombreLignes++;

                sTabResult = new String[m_iNombreLignes, m_iNombreColonnes];

                myReader.Close();
                myReader = myCMD.ExecuteReader();

                int i = 0;


                while (myReader.Read())
                {
                    for (int j = 0; j < m_iNombreColonnes; j++)
                        if (myReader.IsDBNull(j) == false)
                        {
                            switch (j)
                            {
                                case 0:
                                    //if (iChoixProcSto == 1)
                                    //    sTabResult[i, j] = FormatInteg(myReader.GetOracleValue(j).ToString());
                                    //else
                                    sTabResult[i, j] = myReader.GetOracleValue(j).ToString();
                                    break;
                                case 1:
                                    sTabResult[i, j] = myReader.GetOracleValue(j).ToString();
                                    break;
                                case 2:
                                    sTabResult[i, j] = (Convert.ToInt32(myReader.GetOracleValue(j).ToString()) - 1).ToString();
                                    break;
                                default:
                                    sTabResult[i, j] = myReader.GetOracleValue(j).ToString();
                                    break;
                            }
                        }
                        else
                        {
                            sTabResult[i, j] = ""; // Met "" dans les champs null
                        }
                    i++;
                }


                dt = new DataTable();
                DataView dv = dt.DefaultView;

                // choix == 1
                if (z == 1)
                {
                    DataColumn dc;
                    dc = new DataColumn("Code Secteur", Type.GetType("System.String"));
                    dt.Columns.Add(dc);
                    dc = new DataColumn("Dimension", Type.GetType("System.String"));
                    dt.Columns.Add(dc);
                    dc = new DataColumn("Niveau", Type.GetType("System.Int32"));
                    dt.Columns.Add(dc);


                    for (int h = 0; h < i; h++)
                    {
                        DataRow newRow = dt.NewRow();

                        for (int col = 0; col < m_iNombreColonnes; col++)
                        {
                            newRow[col] = sTabResult[h, col];
                        }

                        dt.Rows.Add(newRow);
                    }

                    dv.Sort = "Code Secteur ASC";
                }

                // choix == 2
                if (z == 2)
                {
                    DataColumn dc;
                    dc = new DataColumn("Numero Annonceur", Type.GetType("System.Int32"));
                    dt.Columns.Add(dc);
                    dc = new DataColumn("Nom Annonceur", Type.GetType("System.String"));
                    dt.Columns.Add(dc);

                    for (int k = 0; k < i; k++)
                    {
                        DataRow newRow = dt.NewRow();

                        for (int col = 0; col < m_iNombreColonnes; col++)
                        {
                            newRow[col] = sTabResult[k, col];
                        }

                        dt.Rows.Add(newRow);
                    }
                }

                // return dv;
                try
                {
                    if (z == 1)
                        CreateCSVFile(dv, strNomFichierSecteur);
                    else
                        CreateCSVFile(dv, strNomFichierAnnonceur);


                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message + " avec Arguments : " + strSqlArgs, "Probleme de generation de deux fichiers");
                    return;
                    //Application.Exit();                	
                }
                finally
                {
                    myReader.Close();
                }

            }

            connec.Close();


        }


        private void CreateCSVFile(DataView dview, string strFilePath)
        {
            // Create the CSV file to which grid data will be exported.
            System.IO.StreamWriter sw = new System.IO.StreamWriter(strFilePath, false);

            int iColCount = dview.Table.Columns.Count;

            for (int i = 0; i < iColCount; i++)
            {
                sw.Write(dview.Table.Columns[i]);

                if (i < iColCount - 1)
                {
                    sw.Write(";");
                }
            }

            sw.Write(sw.NewLine);

            // Now write all the rows.

            foreach (DataRowView dr in dview)
            {
                for (int i = 0; i < iColCount; i++)
                {
                    if (!Convert.IsDBNull(dr[i]))
                    {

                        sw.Write(dr.Row[i].ToString());

                    }

                    if (i < iColCount - 1)
                    {
                        sw.Write(";");
                    }
                }

                sw.Write(sw.NewLine);
            }
            sw.Close();
        }


        public static byte[] ReadFile(string filePath)
        {
            byte[] buffer;
            FileStream fileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read);
            try
            {
                int length = (int)fileStream.Length;  // get file length
                buffer = new byte[length];            // create buffer
                int count;                            // actual number of bytes read
                int sum = 0;                          // total number of bytes read

                // read until Read method returns 0 (end of the stream has been reached)
                while ((count = fileStream.Read(buffer, sum, length - sum)) > 0)
                    sum += count;  // sum is a buffer offset for next reading
            }
            finally
            {
                fileStream.Close();
            }
            return buffer;
        }

        
        private void ReadWriteStreamBuffer( OracleConnection connection, string fichier)
        {
            int Length = 32768;
            Byte[] bufferEnd = new Byte[Length];
            
            
            byte[] bufferImport = ReadFile(fichier + ".csv");

            string str;
            ASCIIEncoding enc = new ASCIIEncoding();
            str = enc.GetString(bufferImport);

            int nbLignesEntrant = 0;
            string nbLignesDonne = "" ;
           
            string[] sep = { "\r\n" };
            string[] tabRecu = str.Split(sep,StringSplitOptions.None);

            //StringBuilder sb = new StringBuilder();
            int lenBufferPosition = 0;
            // bufferCurrent = new Byte[16000];

            foreach (string code in tabRecu)
            {
                try
                {
                    if (code.Length > 6)
                    {
                        //sb.Append(RequeteUnEcranDispo(code, connection).Replace("-", "") + ";" + "\r\n");
                                             
                        byte[] bufferCurrent = new Byte[32];
                        bufferCurrent = Encoding.UTF8.GetBytes(RequeteUnEcranDispo(code, connection) + "\r\n");//.Replace("-", "")

                        Buffer.BlockCopy(bufferCurrent, 0 , bufferEnd, lenBufferPosition, Buffer.ByteLength(bufferCurrent));

                        lenBufferPosition += bufferCurrent.Length;

                        nbLignesEntrant++;
                    }
                    else
                    {
                        if(!String.IsNullOrEmpty(code))
                        {                            
                            nbLignesDonne =code ;
                        }
                    }
                }
                catch (System.Exception ex)
                {
                    string mess = ex.Message + " avec comme arguments : " + strSqlArgs;
                    throw ex;
                }
            }


            byte[] bufferCurrentFin = new Byte[32]; ;

            bufferCurrentFin = Encoding.UTF8.GetBytes("[" + nbLignesEntrant + "]" + "\r\n");

            Buffer.BlockCopy(bufferCurrentFin, 0, bufferEnd, lenBufferPosition, Buffer.ByteLength(bufferCurrentFin));

            //sb.Append("[" + nbLignesEntrant + "]");
            //StreamWriter sw = new StreamWriter(fichier + "_R.csv");
            //sw.Write(sb.ToString());

            // create a write stream
            FileStream writeStream = new FileStream(fichier + "_R.csv", FileMode.Create, FileAccess.Write, FileShare.None);//, FileOptions.SequentialScan

            writeStream.Write(bufferEnd, 0, lenBufferPosition  + bufferCurrentFin.Length);

            writeStream.Close();

            Thread.Sleep(1000);

            //Timer au cas ou le nombre d'enregistrement est faux
            DateTime TempsDebut;
            TempsDebut= DateTime.Now;
            TimeSpan delta;
            DateTime  TempsFin;


            while ("[" + nbLignesEntrant + "]" != nbLignesDonne)
            {
                Thread.Sleep(1000);
                TempsFin = DateTime.Now;
                delta = TempsFin - TempsDebut;
                if(delta.Seconds > 30 )
                {
                    return;
                }

            }                       

        }


        private void ReadWriteStream( OracleConnection connection, string fichier)
        {
            int Length = 32700;
            Byte[] buffer = new Byte[Length];
            int i = 0;
            int len = 0;
            StreamReader monStreamReader = new StreamReader(fichier + ".csv");
            string ligne = monStreamReader.ReadLine();
            string nbLignesReader = "";

            StringBuilder sb = new StringBuilder();
          

            while (ligne !=null)// && bytesRead > 0
            {
                if (!monStreamReader.EndOfStream)
                 {

                    //byte[] b1 = new byte[32]; 
                    //b1 = Encoding.UTF8.GetBytes(RequeteUnEcranDispo(ligne, connection).Replace("-", "") + "\r\n");
                    sb.Append(RequeteUnEcranDispo(ligne, connection).Replace("-", "") + ";" + "\r\n");
                   
                }
                if (monStreamReader.EndOfStream)
                {
                       nbLignesReader = ligne;
                }
                ligne = monStreamReader.ReadLine();
                i++;
                len++;
            }

            if (nbLignesReader.Contains("["))
            {
                ligne = nbLignesReader.Substring(1, nbLignesReader.Length - 2);
            }

            if (!(ligne == (len-1).ToString()))
            {
                MessageBox.Show("Le nombre de lignes est different entre le fichier entrant et sortant", "Probleme de fichier txt");
            }

            string tot = "[" + (i - 1).ToString() + "]";

            sb.Append(tot);

            StreamWriter sw = new StreamWriter(fichier + "_R.csv");

            sw.Write(sb.ToString());
            sw.Close();
            monStreamReader.Close();


            bool end = true;

            while (end)
            {
                try
                {
                    var lignes = File.ReadAllLines(fichier + "_R.csv");
                    end = false;
                }
                catch (System.Exception ex)
                {
                	Thread.Sleep(1000);
                }                
                    
            }


        
            

        }


        public bool lectureEcritureFichierEcranDispo(string fichier, OracleConnection connection)
        {
            try
            {
                // create a write stream
                //FileStream writeStream = new FileStream(fichier + "_R.csv", FileMode.Create, FileAccess.Write, FileShare.None);//, FileOptions.SequentialScan
                // write to the stream
                ReadWriteStreamBuffer(connection,fichier);

                connection.Close();
                connection.Dispose();               

            }
            catch (Exception ex)
            {
                string err = ex.Message;
                MessageBox.Show(err, "Probleme de connexion");
                throw ex; 
            }

            return true;
        }
        
        /// <summary>
        /// retourne un code ercan en fonction de la string parametres a decouper pour la fonction oracle FN_GET_ECRAN_DISPO
        /// </summary>
        /// <param name="parametres">string d'entree a splitter</param>
        /// <returns></returns>
        public string RequeteUnEcranDispo(string parametres, OracleConnection conn)
        {
            string strResult = "0";

            try
            {
                //OracleDataReader myReader;
                OracleCommand cmd;
                strResult = "0";
                string[] strCh = parametres.Split(';');
                cmd = new OracleCommand("NELIS.F_JFC_GET_ECRAN_DISPO", conn);
                cmd.CommandType = CommandType.StoredProcedure;

                //string hdeb = strCh[3].Substring(0, 4);
                //string hfin = strCh[4].Substring(0, 4);
                

                cmd.Parameters.Add("a_regie", OracleType.VarChar, 2).Value = strCh[0].ToString();
                cmd.Parameters.Add("a_region", OracleType.VarChar, 2).Value = strCh[1].ToString();
                cmd.Parameters.Add("d_datdif", OracleType.VarChar, 8).Value = strCh[2].ToString();
                cmd.Parameters.Add("h_debut", OracleType.VarChar, 10).Value = strCh[3].ToString();
                cmd.Parameters.Add("h_fin", OracleType.VarChar, 10).Value = strCh[4].ToString();
                cmd.Parameters.Add("i_format", OracleType.VarChar, 2).Value = strCh[5].ToString();

                cmd.Parameters.Add("RetVal", OracleType.VarChar,10);
                cmd.Parameters["RetVal"].Direction = ParameterDirection.ReturnValue;

                cmd.ExecuteNonQuery(); 
                

                //myReader = cmd.ExecuteReader();
                strResult = (string)cmd.Parameters["RetVal"].Value;// 6
                cmd.Dispose();
                //myReader.Dispose();
                //myReader.Close();

            }
            catch (System.Exception ex)
            {
                strResult = "0";
                //string mess = ex.Message;
                //MessageBox.Show(mess + "ligne erreur :" + strCh[0].ToString() + ";" + strCh[1].ToString() + ";" + strCh[2].ToString() + ";" + strCh[3].ToString() + ";" + strCh[4].ToString() + ";" + strCh[5].ToString() + ";", "Probleme de connexion");
                //throw ex;
            }
            return strResult;
                       
        }




    }
}

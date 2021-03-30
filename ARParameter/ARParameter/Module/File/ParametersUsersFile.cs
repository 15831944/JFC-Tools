using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ARParameter.Module.File
{
    public class ParametersUsersFile
    {
        public struct UserParameters
        {
            public string Compagny
            { get; set; }
            public string ext_raison;
            public string Date;
            public string Hour;
            public string UserPath;
            public bool NetworkMode;
            public string str;
            public string UserTablePath;
            public string Email
            { get; set; }
            public string EDICode;
            public bool NomenclatureAcces;
            public string EDIPath;
            public string EDIPath2;
            public string NomenclaturePath;
            public string NomenclaturePath2;
            public string ExecutableToLaunch;
            public string UpdatePath;
            public string UserName
            { get; set; }
            public string UserInternTablePath;
            public string ExchangeInternPath;
        }

        private string fileName;
        //private Dictionary<string, List<UserParameters>> userParamsByCompagny;
        private List<UserParameters> userParams;

        /// <summary>
        /// Obtient ou définit le chemin d'accès complet au fichier utilisateur.
        /// </summary>
        public string Filename
        {
            get { return fileName; }
            set { fileName = value; }
        }

        //public Dictionary<string, List<UserParameters>> UserParamsByCompagny
        //{
        //    get => userParamsByCompagny;
        //}

        public List<UserParameters> UserParams
        {
            get => userParams;
        }

        /// <summary>
        /// Initialize une instance .
        /// </summary>
        /// <param name="lpFileName">Chemin d'accès complet au fichier utilisateur.</param>
        public ParametersUsersFile(string fileName)
        {
            this.fileName = fileName;
        }

        public void ReadFile()
        {
            if (System.IO.File.Exists(fileName))
            {
                try
                {
                    //userParamsByCompagny = new Dictionary<string, List<UserParameters>>();
                    userParams = new List<UserParameters>();

                    // Open the text file using a stream reader.
                    using (var sr = new StreamReader(fileName))
                    {
                        string compagny = "";
                        string ext_raison = "";
                        //List<UserParameters> userParams;

                        while (sr.Peek() >= 0)
                        {
                            string line = sr.ReadLine();

                            string[] tabLine = line.Split(';');

                            if (tabLine[0] != string.Empty && tabLine.Length <= 2)
                            {
                                compagny = tabLine[0];

                                if (tabLine.Length > 1)
                                    ext_raison = tabLine[1];
                            }
                            else if (compagny != string.Empty && tabLine.Length > 2)
                            {
                                UserParameters up = new UserParameters();

                                up.Compagny = compagny;
                                up.ext_raison = ext_raison;
                                up.Date = tabLine[0];
                                up.Hour = tabLine[1];
                                up.UserPath = tabLine[2];
                                up.NetworkMode = tabLine[3] == "0" ? false : true;
                                up.str = tabLine[4];
                                up.UserTablePath = tabLine[5];
                                up.Email = tabLine[6];
                                up.EDICode = tabLine[7];
                                up.NomenclatureAcces = tabLine[8] == "0" ? false : true;
                                up.EDIPath = tabLine[9];
                                up.EDIPath2 = tabLine[10];
                                up.NomenclaturePath = tabLine[11];
                                up.NomenclaturePath2 = tabLine[12];
                                up.ExecutableToLaunch = tabLine[13];
                                up.UpdatePath = tabLine[14];
                                up.UserName = tabLine[15];
                                up.UserInternTablePath = tabLine[16];
                                up.ExchangeInternPath = tabLine[17];

                                //if (userParamsByCompagny.TryGetValue(compagny, out userParams))
                                //{
                                //    userParams.Add(up);
                                //}
                                //else
                                //{
                                //    userParams = new List<UserParameters>();
                                //    userParams.Add(up);
                                //    userParamsByCompagny.Add(compagny, userParams);
                                //}

                                userParams.Add(up);

                            }
                        }
                    }
                }
                catch (IOException e)
                {
                    Console.WriteLine("The file could not be read:");
                    Console.WriteLine(e.Message);
                }
            }
        }
    }
}

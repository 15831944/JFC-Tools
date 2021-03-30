using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ARParameter.Module.File
{
    public class UserFile
    {
        public struct Compagny
        {
            //Havas Média France
            public string Name;

            //\\fr-par-cfs02\mres\ArtRadio\share\Havas
            public string UserPath;

            //\\fr-par-cfs02\mres\ArtRadio\EDI
            public string EDIPath;

            //HACG2
            public string ComptoireCode;

            //\\fr-par-cfs02\mres\ArtRadio\nomenclh\
            public string NomenclaturePath;

            //genevieve.charron @havasmedia.com
            public string UserEmail;

            //RAISON.HVM
            public string RaisonFile;

        }

        private string fileName;
        private List<Compagny> compagnies;

        public List<Compagny> Compagnies
        {
            get => compagnies;
        }

        /// <summary>
        /// Obtient ou définit le chemin d'accès complet au fichier utilisateur.
        /// </summary>
        public string Filename
        {
            get { return fileName; }
            set { fileName = value; }
        }

        /// <summary>
        /// Initialize une instance.
        /// </summary>
        /// <param name="lpFileName">Chemin d'accès complet au fichier utilisateur.</param>
        public UserFile(string fileName)
        {
            this.fileName = fileName;
            compagnies = new List<Compagny>();
        }

        public void ReadFile()
        {
            if (System.IO.File.Exists(fileName))
            {
                try
                {
                    // Open the text file using a stream reader.
                    using (var sr = new StreamReader(fileName))
                    {
                        while (sr.Peek() >= 0)
                        {
                            string line = sr.ReadLine();

                            string[] tabLine = line.Split(';');

                            if (tabLine.Length == 7)
                            {
                                Compagny cp = new Compagny();
                                cp.Name = tabLine[0];
                                cp.UserPath = tabLine[1];
                                cp.EDIPath = tabLine[2];
                                cp.ComptoireCode = tabLine[3];
                                cp.NomenclaturePath = tabLine[4];
                                cp.UserEmail = tabLine[5];
                                cp.RaisonFile = tabLine[6];

                                compagnies.Add(cp);
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

        public void SaveFile()
        {
            if (System.IO.File.Exists(fileName))
            {
                FileInfo fi = new FileInfo(fileName);

                string newFileName = "";

                newFileName = fileName + "." + DateTime.Now.ToString("yyyyMMdd");

                if (System.IO.File.Exists(newFileName))
                {
                    System.IO.File.Delete(newFileName);
                }

                fi.MoveTo(newFileName);
            }

            try
            {
                // Open the text file using a stream reader.
                using (var sr = new StreamWriter(fileName, false))
                {
                    foreach (var cp in compagnies)
                    {
                        string line = cp.Name + ";" + cp.UserPath + ";" + cp.EDIPath + ";" + cp.ComptoireCode + ";" + cp.NomenclaturePath + ";" + cp.UserEmail + ";" + cp.RaisonFile;

                        sr.WriteLine(line);
                    }
                }
            }
            catch (IOException e)
            {
                Console.WriteLine("The file could not be write:");
                Console.WriteLine(e.Message);
            }

        }
    }
}

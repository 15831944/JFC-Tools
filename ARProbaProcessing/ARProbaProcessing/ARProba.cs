using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace ARProbaProcessing
{
    public class ARProba
    {
        static void Main(string[] args)
        {
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //
            // - créer les dossiers Input et Output
            // - créer le dossier Input\Bde
            // - créer le dossier Input\Xls
            //
            // - Copier les fichiers de l'affinage (fournis par Guillaume) vers le nouvel affinage, dans Input\ (3 fichiers U181*.*)
            // - Copier le fichier de l'affinage précédent vers le nouvel affinage, dans Input\ (U181_sta_exception.desc)
            // - Copier les fichiers de :
            //    V:\Donnee\Radio\France\Fichiers_Bruts_Instituts\Panel National\2018\Installation\DI-Panel-Radio-cumul-2016-2018.zip\Jn\Jn\F_aud18.zip\St*.bde
            //   vers :
            //    Input\Bde
            //
            // - Copier les fichiers de Input\Xls de l'ancien affinage vers le nouveau
            // - Ouvrir le fichier Input\Xls\sta_list.csv. Remplacer les données par la liste des stations présente dans :
            //    V:\Donnee\Radio\France\Fichiers_Bruts_Instituts\Panel National\2018\Installation\DI-Panel-Radio-cumul-2016-2018.zip\LISEZMOI_2018.doc
            // - Ouvrir le fichier Input\Xls\lib_sig.csv. Remplacer les données par les données du fichier (onglet LIBELLE) :
            //    V:\Donnee\Radio\France\Fichiers_Bruts_Instituts\Panel National\2018\Installation\DI-Panel-Radio-cumul-2017-2018.zip\Sign_Livraison\stru_sign_p2018.xlsx
            // - Ouvrir le fichier Input\Xls\str_sig.csv. Remplacer les données par les données du fichier (onglet Structure) :
            //    V:\Donnee\Radio\France\Fichiers_Bruts_Instituts\Panel National\2018\Installation\DI-Panel-Radio-cumul-2017-2018.zip\Sign_Livraison\stru_sign_p2018.xlsx
            //
            // - Copier le fichier :
            //    V:\Donnee\Radio\France\Fichiers_Bruts_Instituts\Panel National\2018\Installation\DI-Panel-Radio-cumul-2017-2018.zip\Sign_Livraison\sign2018.bde
            //   vers :
            //    Input\Bde\sig18jfc.bde
            //
            // - Ouvrir le fichier Input\U181_sta_exception.desc, et mettre en correspondance les libellés des stations avec ceux de xls\sta_list.csv
            //
            // - Modifier les lignes du programme pour mettre à jour les dates et dossiers
            //
            // - Commenter le process.TestCouv()
            // - Dans la méthode Run(), supprimer les commentaires du f04Processor
            // - Dans la méthode Run(), mettre le flag run à false
            //
            // - Mettre des points d'arrêt sur toutes les exceptions de la méthode LoadSationList()
            // - Exécuter en Debug, et remplacer tous les libellés de stations dans le fichier lib_sig.csv
            //
            // - Remettre les commentaires du f04Processor de la méthode Run()
            // - Dans la méthode Run(), mettre le flag run à true
            //
            // - Supprimer les commentaires du process.TestCouv()
            // - Relancer en Release
            // 
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            bool ModeDebug = false;

            FAffinage fAffinage = new FAffinage();
            if (fAffinage.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {

                // PANEL CADRES
                //FilterNatToCadre(@"D:\Work\AR\Probabilisation\FR\Panel Cadre\Panfra", "13");
                //return;
                int year = fAffinage.GetYear();
                Console.WriteLine(2019.ToString("0000") + " ==>");
                string panelName = fAffinage.GetDirName(); // "Panel_Cadre";
                Enquete enquete = fAffinage.GetEnquete();

                // PANEL NATIONAL
                var process = new ARProba(@"C:\Affinage\" + panelName + @"\Panfra", (year % 100).ToString("00"), "", enquete);

                // PANEL IDF
                //var process = new ARProba(@"C:\Affinage\Panel_Idf\Panfra", "20", "");
                //            process.Run();

                string inputDir = @"c:\Affinage\" + panelName + @"\Panfra" + (year % 100).ToString("00") + @"\Input";
                string outputDir = @"c:\Affinage\" + panelName + @"\Panfra" + (year % 100).ToString("00") + @"\OutputC";

                if (!Directory.Exists(outputDir)) Directory.CreateDirectory(outputDir);

                new AffinageProcess().Run(process,
                    enquete,
                    inputDir,
                    outputDir
                    );

                return;
            }

            //process.Run();
            //if (!ModeDebug)
            //    process.TestCouv();    // en 1ere phase en commentaire (Debug) et mettre en 2eme phase en mode Release

            // HAVAS - 1/2 HEURE
            // process.ExportControl();

            //process.CutSupFileIDF_NAT();

            //process = new ARProba(@"C:\Work\AR\Probabilisation\FR\Panel National\Panfra", "09");
            ////process.Run();
            //process.TestCouv();


            Console.WriteLine("Process done: Press a key...");
            Console.In.ReadLine();

        }

        public static void FilterNatToCadre(string path, string yy)
        {
            string root = path + yy;

            string[] bdecadre = File.ReadAllLines(Path.Combine(root, @"Input\Bde\Pan" + yy + "_DI-cadres.bde"));


            HashSet<string> cadreMap = new HashSet<string>();
            // Load sample id dic for cadre
            Dictionary<string, string> cadreDic = new Dictionary<string, string>();
            foreach (var line in bdecadre)
            {
                string key = line.Substring(0, 8);
                cadreDic.Add(key, line);
                cadreMap.Add(key);
            }

            // Convert audience files
            var audfiles = Directory.EnumerateFiles(Path.Combine(root, @"Input\BdeNat"), "st*.bde");

            foreach (var audfile in audfiles)
            {
                var inls = File.ReadAllLines(audfile);
                List<string> outls = new List<string>();

                foreach (var line in inls)
                {
                    string key = line.Substring(0, 8);
                    if (cadreMap.Contains(key))
                    {
                        outls.Add(line);
                    }
                }

                File.WriteAllLines(Path.Combine(root, @"Input\Bde", Path.GetFileName(audfile)), outls);
            }


            string[] bdenat = File.ReadAllLines(Path.Combine(root, @"Input\BdeNat\sig" + yy + "jfc.bde"));

            List<string> sigout = new List<string>();

            // Load sample id dic for nat
            //Dictionary<string, string> natDic = new Dictionary<string, string>();
            foreach (var line in bdenat)
            {
                string key = line.Substring(0, 8);
                string cadreline;
                if (cadreDic.TryGetValue(key, out cadreline))
                {
                    sigout.Add(line + cadreline[cadreline.Length - 1]);
                }
            }

            File.WriteAllLines(Path.Combine(root, @"Input\Bde\sig" + yy + "jfc.bde"), sigout);

        }

        #region Input/Output path...

        public string InputPath
        { get; private set; }

        public string OutputPath
        { get; private set; }

        public string ProgramPath
        { get; private set; }

        public string InputShortPath
        { get; private set; }

        public string OutputShortPath
        { get; private set; }

        public string ProgramShortPath
        { get; private set; }

        #endregion

        public int IndivCount
        { get; private set; }

        public int DayCount
        { get; private set; }

        public string YearName
        { get; private set; }

        #region Station List...

        public IEnumerable<Station> StationList
        { get; private set; }

        public int StationCount
        { get { return StationList.Count(); } }

        public IEnumerable<Station> HabAndNotoStationList
        { get; private set; }

        public int HabAndNotoStationCount
        { get { return HabAndNotoStationList.Count(); } }

        public IEnumerable<Station> HabStationList
        { get; private set; }

        public int HabStationCount
        { get { return HabStationList.Count(); } }
        public int AllHabStationCount
        {
            get
            {
                var req = from sta in StationList
                          where (sta.Mode == Station.eSignVariable.Habit) // && sta.Comment != "Non disponible")
                          select sta;
                return req.Count() + hiddenHabStationCount;
            }
        }

        public IEnumerable<Station> NotoStationList
        { get; private set; }

        public int NotoStationCount
        { get { return NotoStationList.Count(); } }

        public IEnumerable<Station> HabAndNotoTotalStationList
        { get; private set; }

        public int HabAndNotoTotalStationListCount
        { get { return HabAndNotoTotalStationList.Count(); } }

        private int hiddenHabStationCount = 0;

        #endregion

        #region U1xx station list handling

        public int U1xxStationCount { get; private set; }
        public int U1xxModalityCount { get; private set; }

        public List<string> U1xxPopTxt { get; private set; }

        public Dictionary<string, int> U1xxModalityVars
        { get; private set; }
        #endregion

        #region Signaletique file...

        public string SigFileName
        { get; private set; }

        public string SigFileNameGrouped
        { get; private set; }

        public string FullSigFileShortName
        { get { return InputShortPath + SigFileNameGrouped; } }

        public string FullSigFileName
        { get { return InputPath + SigFileName; } }

        public string FullSigFileNameGrouped
        { get { return InputPath + SigFileNameGrouped; } }

        public int SigLineLen
        { get; private set; }

        public Dictionary<string, SignVariable> SignVars
        { get; private set; }

        #endregion

        public ARProba(string basePathPrefix, string yearName, string basePathSuffix, Enquete enquete)
        {
            // Init manual parameters

            string basePath = basePathPrefix + yearName + basePathSuffix;

            InputPath = basePath + @"\Input\";
            OutputPath = basePath + @"\Output\";
            ProgramPath = basePath + @"\Program\";
            YearName = yearName;

            //InputPath = @"C:\Work\AR\Probabilisation\FR\Panel National\Panfra10\Input\";
            //OutputPath = @"C:\Work\AR\Probabilisation\FR\Panel National\Panfra10\Output\";

            //ProgramPath = @"C:\Work\AR\Probabilisation\FR\Panel National\Panfra10\Program\";

            //YearName = "10";


            // Computed parameters...

            DayCount = 23; // 5 * 3 + 1 * 4 + 1 * 4 = 23 => 3 LV et 4 SD

            string bdedir = Path.Combine(InputPath, "Bde");
            var files = Directory.GetFiles(bdedir, "st*.bde");

            var defaultFile = files.First();

            int stindex = 1;
            foreach (var stfile in files)
            {
                string fn = Path.GetFileNameWithoutExtension(stfile).ToLower();

                if (fn.Contains(" - copie de "))
                    continue;

                for (; ; )
                {
                    string expected = "st" + stindex.ToString("00");

                    if (expected != fn)
                    {
                        File.Copy(defaultFile, Path.Combine(bdedir, expected + " - Copie de " + Path.GetFileNameWithoutExtension(defaultFile) + ".bde"));
                        File.Copy(defaultFile, Path.Combine(bdedir, expected + ".bde"));
                    }
                    else
                    {
                        break;
                    }
                    stindex++;
                }

                stindex++;
            }

            // Load the station list
            if (enquete != Enquete.PanelIleDeFrance)
                LoadSationList(enquete);
            else
                LoadSationList_IDF(enquete);

            SigFileName = string.Format(@"Bde\sig{0}jfc.bde", YearName);

            SigFileNameGrouped = string.Format(@"Bde\sig{0}jfc2.bde", YearName);

            // Probe 
            ProbeSigLineLenAndIndivCount();

            // Get the short path name
            StringBuilder buffer = new StringBuilder(256);
            GetShortPathName(InputPath, buffer, buffer.Capacity);

            InputShortPath = buffer.ToString();

            buffer.Clear();
            GetShortPathName(OutputPath, buffer, buffer.Capacity);

            OutputShortPath = buffer.ToString();

            buffer.Clear();
            GetShortPathName(ProgramPath, buffer, buffer.Capacity);

            ProgramShortPath = buffer.ToString();

            LoadSignStructure(enquete);

        }



        #region Load station List
        private void LoadSationList(Enquete enquete)
        {
            // Load Station list
            string staListFile = InputPath + string.Format(@"xls\sta_list.csv");

            List<Station> list = new List<Station>();

            var staList = File.ReadAllLines(staListFile, Encoding.GetEncoding("Windows-1252"));
            foreach (var sta in staList)
            {
                if (sta.Equals(string.Empty))
                    continue;

                var words = sta.Split(';');
                if (words.Length < 2)
                    throw new Exception("Error while loading the file " + staListFile);

                int id = int.Parse(words[0]);
                string name = words[1].Trim();

                string comment = null;
                if (name.Contains('('))
                {
                    comment = name.Substring(name.IndexOf('('));
                    name = name.Substring(0, name.IndexOf('(')).Trim();
                }
                Station staItem = new Station(id, name);

                if (words[2] != null && !words[2].Equals(string.Empty))
                    staItem.Comment = words[2];
                else if (comment != null)
                    staItem.Comment = comment;

                list.Add(staItem);
            }

            // Load Habit and Notoriety
            string libSigFile = InputPath + string.Format(@"xls\lib_sig.csv");

            var libList = File.ReadAllLines(libSigFile, Encoding.GetEncoding("Windows-1252"));

            string currentVariable = null;


            foreach (var sta in libList)
            {
                if (sta.Equals(string.Empty))
                    continue;

                var words = sta.Split(';');

                if (!words[0].Equals(string.Empty))
                    currentVariable = words[1].Trim();

                if (currentVariable == null)
                    continue;

                if (currentVariable.Length == 4 && currentVariable.StartsWith("LV"))
                {
                    // Get the Hab index
                    int index = int.Parse(currentVariable.Substring(2));

                    if (!words[2].StartsWith("Habitudes d'écoute Lundi-Vendredi"))
                        throw new Exception("Error variable detection on: " + words[2]);

                    string stationName = words[2].Replace("Habitudes d'écoute Lundi-Vendredi", "").Trim();
                    string stationBaseName = stationName;
                    if (stationName.Contains('('))
                        stationName = stationName.Substring(0, stationName.IndexOf('(')).Trim();

                    stationName = Normalize(stationName, enquete);

                    Station item = list.Find((station) => (NormalizeCompare(stationName, Normalize(station.Name, enquete))));

                    if (item == null)
                        throw new Exception("Error could not find the station: " + stationBaseName);

                    item.Mode = Station.eSignVariable.Habit;
                    item.Index = index;


                    // Go to the next var
                    currentVariable = null;
                }
                else if (currentVariable.Length == 5 && currentVariable.StartsWith("NOTO"))
                {
                    int index = int.Parse(currentVariable.Substring(4)) * 100;

                    if (words[5].Equals(string.Empty))
                    {
                        continue;
                    }

                    int modalityIdx = int.Parse(words[5]);
                    string stationName = words[6];
                    string stationBaseName = stationName;


                    if (stationName.Equals("VIDE"))
                    {
                        continue;
                    }

                    if (stationName.Contains('('))
                        stationName = stationName.Substring(0, stationName.IndexOf('(')).Trim();

                    stationName = Normalize(stationName, enquete);

                    Station item = list.Find((station) => (NormalizeCompare(stationName, Normalize(station.Name, enquete))));

                    if (item == null)
                        throw new Exception("Error could not find the station: " + stationBaseName);

                    //--------------- TEST--------------
                    /* if (item.Mode == Station.eSignVariable.Habit)
                     {
                         int test = 0;
                     }
                     else
                     {
                         item.*Mode = Station.eSignVariable.Notoriety;
                     }*/
                    item.Mode = Station.eSignVariable.Notoriety;
                    item.Index = index + modalityIdx;

                }
                else
                {
                    // Go to the next var
                    currentVariable = null;
                }
            }

            // Read Uxx Audience file
            string uxxFile = "";
            if (int.Parse(YearName) <= 19)
                uxxFile = InputPath + string.Format(EnqueteTools.GetSupFileChar(enquete) + YearName + "1.sup.desc");
            else
                uxxFile = InputPath + string.Format(EnqueteTools.GetSupFileChar(enquete) + "1" + YearName + ".sup.desc");
            //string uxxFile = InputPath + string.Format(@"U" + YearName + "1.sup.desc");
            //string uxxFile = InputPath + string.Format(@"C1" + YearName + ".sup.desc");
            var grpStaFile = File.ReadAllLines(uxxFile, Encoding.GetEncoding("Windows-1252"));

            List<string> u1xxList = new List<string>();

            List<string> u1xxModalityList = new List<string>();

            foreach (var line in grpStaFile)
            {
                if (line == null || line.Trim().Equals(string.Empty))
                    continue;

                var words = line.Split(':');

                if (words.Length > 2)
                {
                    // Position des séparatuers ":"   / correction bug si réponse démo avec un ":"
                    int Pos1er2Point = line.IndexOf(':');
                    int Pos2eme2Point = line.IndexOf(':', Pos1er2Point + 1);

                    // On vérifie que les positions des ":" sont bien 5 et 13  (seul les stations)
                    if (words[2] != null && !words[2].Trim().Equals(string.Empty) && Pos1er2Point == 5 && Pos2eme2Point == 13)
                    {
                        string stationName = words[2].Trim();
                        if (stationName.Contains('('))
                            stationName = stationName.Substring(0, stationName.IndexOf('(')).Trim();
                        u1xxList.Add(stationName);
                    }
                    else if (words[2].Trim().Equals(string.Empty))
                    {
                        // This is a modality
                        u1xxModalityList.Add(words[1].Trim());
                    }
                }
            }

            U1xxStationCount = u1xxList.Count;
            U1xxModalityCount = u1xxModalityList.Count;

            List<string> exceptionList = new List<string>();
            string uxxExceptFile = "";
            if (int.Parse(YearName) <= 19)
                uxxExceptFile = InputPath + string.Format(EnqueteTools.GetSupFileChar(enquete) + YearName + "1_sta_exception.desc");
            else
                uxxExceptFile = InputPath + string.Format(EnqueteTools.GetSupFileChar(enquete) + "1" + YearName + "_sta_exception.desc");

            if (!File.Exists(uxxExceptFile))
            {
                FileStream l_out = File.Create(uxxExceptFile);
                l_out.Close();
            }

            var xceptFile = File.ReadAllLines(uxxExceptFile, Encoding.GetEncoding("Windows-1252"));

            List<string> exceptedExceptionList = new List<string>();

            // Load expected exceptions
            foreach (var except in xceptFile)
            {
                if (except == null || except.Trim().Equals(string.Empty))
                    continue;

                exceptedExceptionList.Add(except.Trim());
            }

            // init Uxx index

            int sud_plus_Wit_idx = u1xxList.FindIndex((str) => (str.Equals("Sud + Wit"))) + 1;

            if (sud_plus_Wit_idx > 0)
                u1xxList[sud_plus_Wit_idx - 1] = null;

            // Get index for all station in audience file U1xx
            foreach (var sta in list)
            {
                if (sta.Mode == Station.eSignVariable.None)
                    continue;

                string stationName = sta.Name;

                if (stationName.Equals("Sud + Wit") || stationName.Equals("Sud Radio"))
                {
                    sta.U1xxIdx = sud_plus_Wit_idx;
                    continue;
                }

                stationName = Normalize(stationName, enquete);

                int idx = 1;
                foreach (var station in u1xxList.ToArray())
                {
                    if (station != null)
                    {
                        if (NormalizeCompare(stationName, Normalize(station, enquete)))
                        {
                            sta.U1xxIdx = idx;
                            u1xxList[idx - 1] = null;
                            break;
                        }
                    }
                    idx++;
                }

                if (sta.U1xxIdx == 0 && sta.Mode != Station.eSignVariable.None)
                {
                    // Try again with more normalization

                    stationName = sta.Name;
                    stationName = Normalize2(stationName, enquete);

                    idx = 1;
                    foreach (var station in u1xxList.ToArray())
                    {
                        if (station != null)
                        {
                            string nors = Normalize2(station, enquete);
                            if (NormalizeCompare(stationName, nors))
                            {
                                sta.U1xxIdx = idx;
                                u1xxList[idx - 1] = null;
                                break;
                            }
                        }
                        idx++;
                    }
                }

                if (sta.U1xxIdx == 0 && sta.Mode != Station.eSignVariable.None)
                {
                    if (!exceptedExceptionList.Contains(sta.Name))
                        exceptionList.Add(sta.Name);
                }
            }

            if (exceptionList.Count > 0)
            {
                using (var l_file = new StreamWriter(Path.Combine(InputPath, "Exception_provenantducode.desc")))
                {
                    foreach (String i_exception in exceptionList)
                    {
                        l_file.WriteLine(i_exception);
                    }
                    l_file.Close();
                }

                throw new Exception("Exception: " + exceptionList.ToString());
            }

            // Set IDF flag if any
            foreach (var sta in list)
            {
                if (sta.Mode == Station.eSignVariable.None)
                    continue;

                string stationName = sta.Name;

                stationName = Normalize(stationName, enquete);

                int idx = 1;
                foreach (var station in u1xxList.ToArray())
                {
                    if (station != null)
                    {
                        if (NormalizeCompare(stationName, Normalize(station, enquete)))
                        {
                            sta.IsIdf = true;
                            u1xxList[idx - 1] = null;
                            break;
                        }
                    }
                    idx++;
                }

                if (!sta.IsIdf)
                {
                    // Try again with more normalization

                    stationName = sta.Name;
                    stationName = Normalize2(stationName, enquete);

                    idx = 1;
                    foreach (var station in u1xxList.ToArray())
                    {
                        if (station != null)
                        {
                            string nors = Normalize2(station, enquete);
                            if (NormalizeCompare(stationName, nors))
                            {
                                sta.IsIdf = true;
                                u1xxList[idx - 1] = null;
                                break;
                            }
                        }
                        idx++;
                    }
                }

            }

            //var test = (from st in list where st.Mode != Station.eSignVariable.None select st.Name).ToArray();
            //var test1 = (from st in list where st.IsIdf select st.Name).ToArray();
            //var test2 = (from stn in u1xxList where stn != null select stn).ToArray();

            // Load population txt
            U1xxPopTxt = new List<string>();

            string uxxPopFile = "";
            if (int.Parse(YearName) <= 19)
                uxxPopFile = InputPath + string.Format(EnqueteTools.GetSupFileChar(enquete) + YearName + "1_Pop.txt");
            else
                uxxPopFile = InputPath + string.Format(EnqueteTools.GetSupFileChar(enquete) + "1" + YearName + "_Pop.txt");

            var popTxtFile = File.ReadAllLines(uxxPopFile, Encoding.GetEncoding("Windows-1252"));

            foreach (var line in popTxtFile)
            {
                if (line == null || line.Trim().Equals(string.Empty))
                    continue;

                var words = line.Split(':');

                U1xxPopTxt.Add(words[1].Trim());
            }

            if (U1xxPopTxt.Count != 3)
                throw new Exception("txt population file error");

            // Init the fitred list

            StationList = list;

            HabAndNotoStationList = (from sta in list
                                     where sta.Mode != Station.eSignVariable.None
                                        && !sta.Name.Equals("total TV", StringComparison.CurrentCultureIgnoreCase)
                                        && !sta.Name.Equals("total radio", StringComparison.CurrentCultureIgnoreCase)
                                     select sta).ToArray();

            HabAndNotoTotalStationList = (from sta in list
                                          where sta.Mode != Station.eSignVariable.None
                                             && (sta.Name.Equals("total TV", StringComparison.CurrentCultureIgnoreCase)
                                                 || sta.Name.Equals("total radio", StringComparison.CurrentCultureIgnoreCase)
                                                 )
                                          select sta).ToArray();

            NotoStationList = (from sta in HabAndNotoStationList
                               where sta.Mode == Station.eSignVariable.Notoriety
                               select sta).ToArray();
            HabStationList = (from sta in HabAndNotoStationList
                              where sta.Mode == Station.eSignVariable.Habit
                              select sta).ToArray();

        }


        // Load Stations Liste "IDF"
        private void LoadSationList_IDF(Enquete enquete)
        {
            // Load Station list
            string staListFile = InputPath + string.Format(@"xls\sta_list.csv");

            List<Station> list = new List<Station>();

            var staList = File.ReadAllLines(staListFile, Encoding.GetEncoding("Windows-1252"));
            foreach (var sta in staList)
            {
                if (sta.Equals(string.Empty))
                    continue;

                var words = sta.Split(';');
                if (words.Length < 2)
                    throw new Exception("Error while loading the file " + staListFile);

                int id = int.Parse(words[0]);
                string name = words[1].Trim(' ', '"', '\t');

                string comment = null;
                if (name.Contains('('))
                {
                    comment = name.Substring(name.IndexOf('('));
                    name = name.Substring(0, name.IndexOf('(')).Trim();
                }
                Station staItem = new Station(id, name);

                if (words.Length > 2 && words[2] != null && !words[2].Equals(string.Empty))
                    staItem.Comment = words[2];
                else if (comment != null)
                    staItem.Comment = comment;

                list.Add(staItem);
            }

            // Load Habit and Notoriety
            string libSigFile = InputPath + string.Format(@"xls\lib_sig.csv");

            var libList = File.ReadAllLines(libSigFile, Encoding.GetEncoding("Windows-1252"));

            string currentVariable = null;

            foreach (var sta in libList)
            {

                if (sta.Equals(string.Empty))
                    continue;

                var words = sta.Split(';');

                if (!words[0].Equals(string.Empty))
                    currentVariable = words[1].Trim();

                if (currentVariable == null)
                    continue;

                if (currentVariable.Length == 4 && currentVariable.StartsWith("LV"))
                {
                    // Get the Hab index
                    int index = int.Parse(currentVariable.Substring(2));

                    if (!words[2].StartsWith("Habitudes d'écoute Lundi-Vendredi"))
                        throw new Exception("Error variable detection on: " + words[2]);

                    string stationName = words[2].Replace("Habitudes d'écoute Lundi-Vendredi", "").Trim();
                    string stationBaseName = stationName;
                    if (stationName.Contains('('))
                        stationName = stationName.Substring(0, stationName.IndexOf('(')).Trim();

                    /////////////////////////////// SPECIFIQUE IDF /////////////////////
                    if (stationName.Equals("SUD RADIO"))
                    {
                        // Go to the next var because SUB RADIO has been removed from the panel IDF
                        currentVariable = null;
                        hiddenHabStationCount++;
                        continue;
                    }

                    if (stationName.Equals("RADIO CLASSIQUE") && words[7] != null && words[7].Contains("VIDE"))
                    {
                        // Go to the next var because RADIO CLASSIQUE has been removed from the panel IDF AND ADDED back on a different position
                        currentVariable = null;
                        //hiddenHabStationCount++;
                        continue;
                    }

                    stationName = Normalize2(stationName, enquete);
                    ////////////////////////////////////////////////////////////////////////////

                    Station item = list.Find((station) => (NormalizeCompare(stationName, Normalize2(station.Name, enquete))));

                    if (item == null)
                        throw new Exception("Error could not find the station: " + stationName);

                    item.Mode = Station.eSignVariable.Habit;
                    item.Index = index;


                    // Go to the next var
                    currentVariable = null;
                }
                else if (currentVariable.Length >= 4 && currentVariable.StartsWith("NOTO"))
                {
                    int index = 0;
                    if (currentVariable.Length > 4)
                    {
                        index = int.Parse(currentVariable.Substring(4)) * 100;
                    }

                    if (words[5].Equals(string.Empty))
                    {
                        continue;
                    }

                    int modalityIdx = int.Parse(words[5]);
                    string stationName = words[6];

                    if (stationName == "VIDE")
                        continue;

                    string baseStationName = stationName;

                    if (stationName.Contains('('))
                        stationName = stationName.Substring(0, stationName.IndexOf('(')).Trim();

                    if (stationName.Equals("Tout Paris")
                        || stationName.Equals("PUBAUDIO IDF")
                        //|| stationName.Equals("ADO FM")
                        || stationName.Equals("LE MOUV")
                        //|| stationName.Equals("VOLTAGE")
                        || stationName.Equals("RFI")
                        || (stationName.Equals("Radio Classique") && words[7] != null && words[7].Contains("SUPPRESSION"))
                    )
                    {
                        // Go to the next var because this station has been removed from the panel IDF
                        continue;
                    }

                    stationName = Normalize(stationName, enquete);

                    Station item = list.Find((station) => (NormalizeCompare(stationName, Normalize(station.Name, enquete))));

                    if (item == null)
                        throw new Exception("Error could not find the station: " + stationName);

                    item.Mode = Station.eSignVariable.Notoriety;
                    item.Index = index + modalityIdx;

                }
                else
                {
                    // Go to the next var
                    currentVariable = null;
                }
            }

            // Read Uxx Audience file
            //string uxxFile = Path.Combine(InputPath, string.Format(@"R1" + YearName + ".tab"));

            var tblStaFileNat = File.ReadAllLines(Path.Combine(InputPath, "FRSTATIO.TBL"), Encoding.GetEncoding("Windows-1252"));
            var tblStaFileIdf = File.ReadAllLines(Path.Combine(InputPath, "FRSTATIO_IDF.TBL"), Encoding.GetEncoding("Windows-1252"));

            List<string> tblStaFile = new List<string>(tblStaFileIdf);
            tblStaFile.AddRange(tblStaFileNat);

            List<string> u1xxList = new List<string>();
            List<string> notu1xxList = new List<string>();

            List<string> u1xxModalityList = new List<string>();

            string dicAlign = Path.Combine(InputPath, "C1" + YearName + ".sup.desc");
            string[] dicAlignList = File.ReadAllLines(dicAlign, Encoding.GetEncoding("Windows-1252"));

            List<string[]> grpStaFile = new List<string[]>();

            U1xxModalityVars = new Dictionary<string, int>();

            u1xxModalityList.Add("POIDS");
            U1xxModalityVars.Add("POIDS", 1);
            u1xxModalityList.Add("JOURS");
            U1xxModalityVars.Add("JOURS", 2);

            foreach (var line in dicAlignList)
            {
                if (line[0].Equals('\t'))
                    continue;

                string[] ws = line.Split(':');
                if (ws.Length != 3)
                    continue;

                if (ws[2].Trim() == string.Empty)
                {
                    u1xxModalityList.Add(line);
                    U1xxModalityVars.Add(ws[1].Trim(), u1xxModalityList.Count);
                }
                else
                {
                    if (ws[0].Trim() != string.Empty && ws[1].Trim() != string.Empty && ws[2].Trim() != string.Empty)
                        grpStaFile.Add(ws);
                }
            }


            int grpidx = 1;
            foreach (var staCode in grpStaFile)
            {
                string staName = (from sline in tblStaFile
                                  where sline.Length > 50 && sline[0] != ';' && int.Parse(sline.Substring(0, 5)) == int.Parse(staCode[0])
                                  select sline.Substring(19, 30)
                                 ).FirstOrDefault();
                if (staName == null)
                {
                    notu1xxList.Add(staCode + " (" + grpidx + ")");
                    u1xxList.Add(staCode[0] + " : " + staCode[1] + " : " + staCode[2]);
                }
                else
                {
                    //u1xxList.Add("(" + staCode + ")" + staName.Trim() + " (" + grpidx + ")");

                    if (staName.Trim().ToLower() == "sud radio medias idf")
                    {
                        staName = "REGIE 1981 IDF";
                    }

                    u1xxList.Add(staName.Trim());
                }
                grpidx++;
            }

            var listName = list.Select((s) => (s.Name)).ToArray();

            var listProbHabName = list.Where(s => (s.Mode == Station.eSignVariable.Habit)).Select((s) => (s.Name)).ToArray();
            var listProbNotoName = list.Where(s => (s.Mode == Station.eSignVariable.Notoriety)).Select((s) => (s.Name)).ToArray();
            var listNoneName = list.Where(s => (s.Mode == Station.eSignVariable.None)).Select((s) => (s.Name)).ToArray();

            U1xxStationCount = u1xxList.Count;
            U1xxModalityCount = u1xxModalityList.Count;

            List<string> exceptionList = new List<string>();

            // string uxxExceptFile = InputPath + string.Format(@"C1" + YearName + "_sta_exception.desc");
            string uxxExceptFile = GetPathYear(enquete, InputPath, "_sta_exception", "desc");

            var xceptFile = File.ReadAllLines(uxxExceptFile, Encoding.GetEncoding("Windows-1252"));

            List<string> exceptedExceptionList = new List<string>();

            // Load expected exceptions
            foreach (var except in xceptFile)
            {
                if (except == null || except.Trim().Equals(string.Empty))
                    continue;

                exceptedExceptionList.Add(except.Trim());
            }

            // init Uxx index

            int sud_plus_Wit_idx = u1xxList.FindIndex((str) => (str.Equals("Sud + Wit"))) + 1;

            if (sud_plus_Wit_idx > 0) u1xxList[sud_plus_Wit_idx - 1] = null;

            // Get index for all station in audience file U1xx
            foreach (var sta in list.ToArray())
            {

                if (sta.Mode == Station.eSignVariable.None)
                    continue;

                string stationName = sta.Name;

                string baseStationName = sta.Name;

                if (stationName.Equals("Sud + Wit") || stationName.Equals("Sud Radio"))
                {
                    sta.U1xxIdx = sud_plus_Wit_idx;
                    continue;
                }

                stationName = Normalize(stationName, enquete);

                int idx = 1;
                foreach (var station in u1xxList.ToArray())
                {
                    if (station != null)
                    {
                        if (NormalizeCompare(stationName, Normalize(station, enquete)))
                        {
                            sta.U1xxIdx = idx;
                            u1xxList[idx - 1] = null;
                            break;
                        }
                    }
                    idx++;
                }

                if (sta.U1xxIdx == 0 && sta.Mode != Station.eSignVariable.None)
                {
                    // Try again with more normalization

                    stationName = sta.Name;
                    stationName = Normalize2(stationName, enquete);

                    idx = 1;
                    foreach (var station in u1xxList.ToArray())
                    {
                        if (station != null)
                        {
                            string nors = Normalize2(station, enquete);
                            if (NormalizeCompare(stationName, nors))
                            {
                                sta.U1xxIdx = idx;
                                u1xxList[idx - 1] = null;
                                break;
                            }
                        }
                        idx++;
                    }
                }

                if (sta.U1xxIdx == 0 && sta.Mode != Station.eSignVariable.None)
                {
                    if (!exceptedExceptionList.Contains(sta.Name))
                    {
                        exceptionList.Add(sta.Name);
                    }
                    else
                    {
                        //    list.Remove(sta);
                        //sta.Mode = Station.eSignVariable.None;
                    }
                }
            }

            if (exceptionList.Count > 0)
                throw new Exception("Exception: " + exceptionList.ToString());

            // Load population txt
            U1xxPopTxt = new List<string>();

            // string uxxPopFile = InputPath + string.Format(@"C1" + YearName + "_Pop.txt");
            // TODO
            string uxxPopFile = GetPathYear(enquete, InputPath, "_Pop", "txt");

            var popTxtFile = File.ReadAllLines(uxxPopFile, Encoding.GetEncoding("Windows-1252"));

            foreach (var line in popTxtFile)
            {
                if (line == null || line.Trim().Equals(string.Empty))
                    continue;

                var words = line.Split(':');

                U1xxPopTxt.Add(words[1].Trim());
            }

            if (U1xxPopTxt.Count != 3)
                throw new Exception("txt population file error");

            // Init the fitred list

            StationList = list;


            var habAndNotoStationListNotOrdered = (from sta in list
                                                   where sta.Mode != Station.eSignVariable.None
                                                      && !sta.Name.Equals("total TV", StringComparison.CurrentCultureIgnoreCase)
                                                      && !sta.Name.Equals("total radio", StringComparison.CurrentCultureIgnoreCase)
                                                      && sta.U1xxIdx != 0
                                                   select sta).ToArray();
            bool writeOrder = true;
            if (writeOrder)
            {
                var staNameList = from s in habAndNotoStationListNotOrdered
                                  select s.Name;
                string fileStationList = Path.Combine(OutputPath, "Station List.txt");

                File.WriteAllLines(fileStationList, staNameList);
                HabAndNotoStationList = habAndNotoStationListNotOrdered;
            }
            else
            {
                HabAndNotoStationList = new List<Station>(habAndNotoStationListNotOrdered.Count());
                string fileStationList = Path.Combine(OutputPath, "Station List.txt");
                var staNameList = File.ReadAllLines(fileStationList);

                foreach (var staName in staNameList)
                {
                    bool fund = false;
                    foreach (var sta in habAndNotoStationListNotOrdered)
                    {
                        if (sta.Name.Equals(staName))
                        {
                            ((List<Station>)HabAndNotoStationList).Add(sta);
                            fund = true;
                            break;
                        }
                    }

                    if (!fund)
                        throw new Exception("Could not find the station " + staName);
                }
            }

            var realStaList = list.Where(s => s.Mode != Station.eSignVariable.None).Select(s => s.Name + " (" + s.U1xxIdx + ")").ToArray();


            //HabAndNotoTotalStationList = new Station[0];
            HabAndNotoTotalStationList = (from sta in list
                                          where sta.Mode != Station.eSignVariable.None
                                             && (/*sta.Name.Equals("total TV", StringComparison.CurrentCultureIgnoreCase)
                                                  ||*/
           sta.Name.Equals("total radio", StringComparison.CurrentCultureIgnoreCase)
                                                )
                                          select sta).ToArray();

            NotoStationList = (from sta in HabAndNotoStationList
                               where sta.Mode == Station.eSignVariable.Notoriety
                                    && sta.U1xxIdx != 0
                               select sta).ToArray();
            HabStationList = (from sta in HabAndNotoStationList
                              where sta.Mode == Station.eSignVariable.Habit
                                    && sta.U1xxIdx != 0
                              select sta).ToArray();

        }
        #endregion Load station List

        //change le nom des stations 
        static string Normalize2(string str, Enquete enquete)
        {
            string res = str;

            // Normalisation "NATIONAL ou CADRES"
            if (enquete != Enquete.PanelIleDeFrance)
            {
                res = res.Replace("Métr.", "Métropoles");
                res = res.Replace("RMC INFO", "RMC");
                res = res.Replace("Rires", "Rire");
                res = res.Replace("MFM Radio", "MFM");
                res = res.Replace("MFM RADIO", "MFM");
                res = res.Replace("Lagardère Métropoles", "LAP");
                res = res.Replace("Les Parisiennes", "Paris – IDF +");
            }
            else
            {
                res = res.Replace("Métr.", "Métropoles");
                res = res.Replace("RMC INFO", "RMC");
                res = res.Replace("Rires", "Rire");
                res = res.Replace("Lagardère Métropoles", "LAP");
                res = res.Replace("(avCh.France)", "");
                res = res.Replace("FIP Paris", "FIP");
                res = res.Replace("TSF JAZZ", "TSF");
                res = res.Replace("MFM RADIO", "MFM");
                if (res.IndexOf("VOLT") != -1 && res.IndexOf("VOLTAGE") == -1)
                    res = res.Replace("VOLT", "VOLTAGE");
            }

            return Normalize(res, enquete);
        }

        // Les "Normalize"  NATIONAL ou IDF
        static string Normalize(string str, Enquete enquete)
        {
            StringBuilder buf = new StringBuilder();

            // Normalisation "NATIONAL ou CADRES"
            if (enquete != Enquete.PanelIleDeFrance)
            {
                foreach (var ch in str.Trim().ToUpper())
                {
                    if ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
                        buf.Append(ch);
                    else if (ch == '&')
                        buf.Append("ET");
                    else if (ch == ' ' || ch == '\t')
                    { }
                    else
                        buf.Append('?');
                }
            }
            else
            {
                // Normalisation "IDF"
                str = str.Replace("Métr.", "Métropoles");
                str = str.Replace("Radio Latina", "LATINA");
                str = str.Replace("Ado FM", "ADO");
                str = str.Replace("Voltage FM", "VOLTAGE");

                str = str.Replace("Les Indépendants", "LES INDES RADIOS");
                // str = str.Replace("Les Indés Radio", "LES INDES RADIOS"); sep 2015
                str = str.Replace("Les Indés Radios", "LES INDES RADIOS");
                str = str.Replace("Les Indés Radio", "LES INDES RADIOS");

                str = str.Replace("Sud Média Radio IDF", "SUD RADIO MEDIAS IDF");
                str = str.Replace("Lagardère Métrople IDF", "LAGARDERE METROPOLES IDF");


                // StringBuilder buf = new StringBuilder();

                foreach (var ch in str.Trim().ToUpper())
                {
                    if ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
                        buf.Append(ch);
                    else if (ch == '&')
                        buf.Append("ET");
                    else if (ch == ' ' || ch == '\t')
                    { }
                    else
                        buf.Append('?');
                }

            }
            return buf.ToString();
        }

        static bool NormalizeCompare(string a, string b)
        {
            if (a.Length != b.Length)
                return false;

            for (int i = 0; i < a.Length; i++)
            {
                char ca = a[i];
                char cb = b[i];
                if (ca == '?' || cb == '?')
                    continue;

                if (ca != cb)
                    return false;
            }

            return true;
        }

        private void LoadSignStructure(Enquete enquete)
        {
            string signStrFile = InputPath + string.Format(@"xls\str_sig.csv");

            var vars = new Dictionary<string, SignVariable>();

            var signStrF = File.ReadAllLines(signStrFile, Encoding.GetEncoding("Windows-1252"));
            foreach (var line in signStrF)
            {
                if (line == null || line.Equals(string.Empty))
                    continue;

                var words = line.Split(';');
                if (words.Length < 7)
                    throw new Exception("Sign Str file wrong");

                string varNo = words[0];
                string varName = words[1];
                if (varNo.Equals("N°") && varName.Equals("VARIABLE"))
                    continue;

                if (varName.Equals("-"))
                    continue;

                if (varNo.Equals(string.Empty) && varName.Equals(string.Empty))
                    continue;

                string type = words[2];
                int rep = int.Parse(words[3]);
                int len = int.Parse(words[4]);
                int pos = int.Parse(words[5]);
                string com = words[6];

                vars.Add(varName, new SignVariable(type, rep, len, pos, com));

            }

            List<KeyValuePair<string, SignVariable>> before = new List<KeyValuePair<string, SignVariable>>();
            List<KeyValuePair<string, SignVariable>> after = new List<KeyValuePair<string, SignVariable>>();

            List<SignVariable> hlv = new List<SignVariable>();
            List<SignVariable> hsa = new List<SignVariable>();
            List<SignVariable> hdi = new List<SignVariable>();

            Station totalRadio = HabAndNotoTotalStationList.First();
            int totalIdx = totalRadio.Index;

            if (enquete == Enquete.PanelIleDeFrance)
            {
                //hiddenHabStationCount += HabAndNotoTotalStationList.Count();
                HabAndNotoTotalStationList = new Station[0];
            }

            SignVariable trlv = null;
            SignVariable trsa = null;
            SignVariable trdi = null;

            bool beforeDone = false;

            foreach (KeyValuePair<string, SignVariable> var in vars)
            {
                string varName = var.Key;
                if (varName.StartsWith("LV") || varName.StartsWith("SA") || varName.StartsWith("DI"))
                {
                    int staNb;
                    if (varName.Length == 4 && int.TryParse(varName.Substring(2), out staNb))
                    {
                        beforeDone = true;

                        // see if this is lv sa or di
                        if (varName.StartsWith("LV"))
                        {
                            if (totalIdx == staNb)
                            {
                                trlv = var.Value;
                            }
                            else
                            {
                                hlv.Add(var.Value);
                            }
                        }
                        else if (varName.StartsWith("SA"))
                        {
                            if (totalIdx == staNb)
                            {
                                trsa = var.Value;
                            }
                            else
                            {
                                hsa.Add(var.Value);
                            }
                        }
                        else if (varName.StartsWith("DI"))
                        {
                            if (totalIdx == staNb)
                            {
                                trdi = var.Value;
                            }
                            else
                            {
                                hdi.Add(var.Value);
                            }
                        }
                        continue;
                    }
                }

                if (beforeDone)
                {
                    after.Add(var);
                }
                else
                {
                    before.Add(var);
                }
            }

            // put TOTAL RADIO at the end
            hlv.Add(trlv);
            hsa.Add(trsa);
            hdi.Add(trdi);

            // Group all HAB questions
            var sigLines = File.ReadAllLines(FullSigFileName);
            List<string> sigLinesGrouped = new List<string>();

            StringBuilder buffer = new StringBuilder();
            foreach (var sigl in sigLines)
            {
                buffer.Clear();
                foreach (var item in before)
                {
                    buffer.Append(sigl.Substring(item.Value.Position - 1, item.Value.Length * item.Value.Repetition));
                }
                foreach (var item in hlv)
                {
                    buffer.Append(sigl.Substring(item.Position - 1, item.Length * item.Repetition));
                }
                foreach (var item in hsa)
                {
                    buffer.Append(sigl.Substring(item.Position - 1, item.Length * item.Repetition));
                }
                foreach (var item in hdi)
                {
                    buffer.Append(sigl.Substring(item.Position - 1, item.Length * item.Repetition));
                }
                foreach (var item in after)
                {
                    buffer.Append(sigl.Substring(item.Value.Position - 1, item.Value.Length * item.Value.Repetition));
                }

                while (buffer.Length < SigLineLen)
                {
                    buffer.Append('0');
                }

                sigLinesGrouped.Add(buffer.ToString());
            }

            if (File.Exists(FullSigFileNameGrouped))
                File.Delete(FullSigFileNameGrouped);
            File.WriteAllLines(FullSigFileNameGrouped, sigLinesGrouped);

            int position = 1;

            // Update vars position
            foreach (var item in before)
            {
                item.Value.Position = position;
                position += (item.Value.Length * item.Value.Repetition);
            }
            foreach (var item in hlv)
            {
                item.Position = position;
                position += (item.Length * item.Repetition);
            }
            foreach (var item in hsa)
            {
                item.Position = position;
                position += (item.Length * item.Repetition);
            }
            foreach (var item in hdi)
            {
                item.Position = position;
                position += (item.Length * item.Repetition);
            }
            foreach (var item in after)
            {
                item.Value.Position = position;
                position += (item.Value.Length * item.Value.Repetition);
            }

            var vars2 = new Dictionary<string, SignVariable>();

            // Update vars2
            foreach (var item in before)
            {
                vars2.Add(item.Key, item.Value);
            }
            int idx = 1;
            foreach (var item in hlv)
            {
                vars2.Add("LV" + idx.ToString("00"), item);
                idx++;
            }
            idx = 1;
            foreach (var item in hsa)
            {
                vars2.Add("SA" + idx.ToString("00"), item);
                idx++;
            }
            idx = 1;
            foreach (var item in hdi)
            {
                vars2.Add("DI" + idx.ToString("00"), item);
                idx++;
            }
            foreach (var item in after)
            {
                vars2.Add(item.Key, item.Value);
            }

            // Update hab station list idx
            foreach (var item in HabStationList)
            {
                if (item.Index > totalIdx)
                    item.Index--;
            }
            totalRadio.Index = hdi.Count;


            SignVars = vars2;
        }

        private void ProbeSigLineLenAndIndivCount()
        {
            string sigFileName = FullSigFileName;

            var sig = File.OpenText(sigFileName);

            string line = sig.ReadLine();

            SigLineLen = line.Length;

            int count = 0;
            do
            {
                count++;
                line = sig.ReadLine();
            }
            while (line != null);

            IndivCount = count;

            sig.Close();
        }

        public void Control()
        {
            using (var lwriter = new StreamWriter("control.txt"))
            {
                lwriter.WriteLine("stations NB_STA_ALL_HAB");
                foreach (Station station in this.StationList)
                {
                    lwriter.WriteLine("station " + station.Name + " mode " + station.Mode);
                }
                lwriter.WriteLine("");
                foreach (Station station in this.StationList)
                {
                    if (station.Mode == Station.eSignVariable.Habit)
                        lwriter.WriteLine("habit station " + station.Name);
                }
            }




        }

        public void Run()
        {

            bool ModeDebug = false;

            // Create Output JFC folder if it does not exist
            if (!Directory.Exists(OutputPath + "jfc"))
                Directory.CreateDirectory(OutputPath + "jfc");

            //// Generate the F04 files
            // mettre en commentaire en mode Debug 
            // enlever commentaire en mode Release
            //////////////////// pour ces 2 lignes /////////////////////
            if (!ModeDebug)
            {
                var f04Processor = new F04Processor(this);
                f04Processor.ConvertBDEFiles();
            }
            ////////////////////////////////////////////////////////////

            var fortranProcessor = new FortranProcessor(this);

            // false : en mode Debug
            // true  : en mode Release
            bool run = false;
            if (!ModeDebug)
                run = true;

            this.Control();

            // Appel des différents programmes FORTRAN
            ProcessFortranFile(fortranProcessor, "lecpanel", run);
            ProcessFortranFile(fortranProcessor, "segpanel", run);
            ProcessFortranFile(fortranProcessor, "ecrpan1j", run);
            ProcessFortranFile(fortranProcessor, "regr5jp2", run);
            ProcessFortranFile(fortranProcessor, "fushab09", run);
            ProcessFortranFile(fortranProcessor, "chab1qhp", run);
            ProcessFortranFile(fortranProcessor, "crenonin", run);

            ProcessFortranFile(fortranProcessor, "ecrsegpa", run);
            ProcessFortranFile(fortranProcessor, "CALCREGR", run);
            ProcessFortranFile(fortranProcessor, "calcnivo", run);
            ProcessFortranFile(fortranProcessor, "caud1qhp", run);

            ProcessFortranFile(fortranProcessor, "caudtotp", run);

            ProcessFortranFile(fortranProcessor, "sav1qhpa", run);
            ProcessFortranFile(fortranProcessor, "sav1qhps", run);
            ProcessFortranFile(fortranProcessor, "sav1qhpd", run);

            ProcessFortranFile(fortranProcessor, "cgrp75br", run);
            ProcessFortranFile(fortranProcessor, "cont75br", run);

            ProcessFortranFile(fortranProcessor, "cnzuptse", run);
            ProcessFortranFile(fortranProcessor, "cnzuptsa", run);
            ProcessFortranFile(fortranProcessor, "cnzuptdi", run);

            ProcessFortranFile(fortranProcessor, "attribp2", run);

            ProcessFortranFile(fortranProcessor, "transp08", run);
            ProcessFortranFile(fortranProcessor, "crecib08", run);
            ProcessFortranFile(fortranProcessor, "penetr", run);
            ProcessFortranFile(fortranProcessor, "asympt", run);


            // Make station list
            StringBuilder staList = new StringBuilder();
            staList.AppendLine("=====Nat=======");
            foreach (var station in this.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO"))
                {
                    staList.AppendLine(station.Name + "; " + station.Comment);
                }
            }
            staList.AppendLine("=====IDF=======");
            foreach (var station in this.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO") && station.IsIdf)
                {
                    staList.AppendLine(station.Name + "; " + station.Comment);
                }
            }

            File.WriteAllText(OutputPath + "StationList.txt", staList.ToString());
        }

        public void TestCouv()
        {
            TestProcessor test = new TestProcessor(this);

            //int idx = 0;
            //foreach (var station in this.HabAndNotoStationList)
            //{
            //    //if (!station.Name.ToUpper().Equals("SUD RADIO"))
            //    //if (station.Name.ToUpper().Equals("RTL"))
            //    if (!station.Name.ToUpper().Equals("SUD RADIO"))
            //    {
            //        for (int entry = 0; entry < 24 * 2; entry++)
            //        {
            //            double c = test.GetCouvLV700730(idx, entry);
            //            Console.WriteLine(station.Name + "; L-V ;" + c.ToString("F2"));
            //        }
            //        for (int entry = 24 * 2; entry < (24 * 2) * 2; entry++)
            //        {
            //            double c = test.GetCouvLV700730(idx, entry);
            //            Console.WriteLine(station.Name + "; S ;" + c.ToString("F2"));
            //        }
            //        for (int entry = (24 * 2) * 2; entry < (24 * 2) * 3; entry++)
            //        {
            //            double c = test.GetCouvLV700730(idx, entry);
            //            Console.WriteLine(station.Name + "; D ;" + c.ToString("F2"));
            //        }
            //    }
            //    idx++;
            //}


            int idx = 0;
            foreach (var station in this.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO"))
                {
                    double c = test.ComputeWeeklyCouv(idx);
                    Console.WriteLine(station.Name + ";" + c.ToString("F2"));

                    idx++;
                }
            }
            foreach (var station in this.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO") && station.IsIdf)
                {
                    double c = test.ComputeWeeklyCouv(idx);
                    Console.WriteLine(station.Name + ";" + c.ToString("F2"));
                    idx++;
                }
            }

        }


        private void ExportControl()
        {

            string probaFile = OutputPath + "PANRA1" + YearName + ".SUP";

            var pfs = File.Open(probaFile, FileMode.Open, FileAccess.Read);
            BinaryReader reader = new BinaryReader(pfs);

            int indivCount = IndivCount;
            double[] weight = new double[indivCount];

            double population = 0;

            // Read weight vector
            for (int i = 0; i < indivCount; i++)
            {
                double w = ((double)reader.ReadUInt16());
                weight[i] = w;
                population += w;
            }

            // Read empty vector
            for (int i = 0; i < indivCount; i++)
            {
                reader.ReadUInt16();
            }


            System.Console.Out.WriteLine(population);

            List<string> tbres = new List<string>();
            List<string> dres = new List<string>();

            List<string> detail = new List<string>();


            int idx = 0;
            foreach (var station in this.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO"))
                {

                    ExportControlFor(idx, station.Name, reader, weight, tbres, dres, detail);

                    idx++;
                }
            }
            foreach (var station in this.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO") && station.IsIdf)
                {

                    ExportControlFor(idx, station.Name, reader, weight, tbres, dres, detail);

                    idx++;
                }
            }

            System.Console.Out.WriteLine(string.Format("{0} / {1}", reader.BaseStream.Position, reader.BaseStream.Length));


            File.WriteAllLines(@"c:\Temp\Res-demi-heure-13+.csv", tbres);
            File.WriteAllLines(@"c:\Temp\Res-LV-DI-SA-13+.csv", dres);

            File.WriteAllLines(@"c:\Temp\detail_proba.csv", detail);


            reader.Close();
            pfs.Close();

        }


        private void ExportControlFor(int idx, string stationName, BinaryReader reader, double[] weight, List<string> tbres, List<string> dres, List<string> detail)
        {
            int indivCount = IndivCount;

            System.Console.Out.WriteLine(stationName);

            if (stationName == "Chérie FM")
            {
                string line = "station; type jour;n° 1/2h; proba; indiv";
                System.Console.Out.WriteLine(line);
            }

            foreach (var day in days)
            {

                double[] dayprobas = new double[indivCount];

                foreach (var timeband in timebands)
                {

                    double effectif = 0;

                    // Read weight vector
                    for (int i = 0; i < indivCount; i++)
                    {
                        double p = ((double)reader.ReadUInt16()) / 1000.0;
                        effectif += p * weight[i];

                        dayprobas[i] = 1.0 - ((1.0 - dayprobas[i]) * (1.0 - p));

                        if (stationName == "Chérie FM")
                        {
                            string lined = string.Format("{0}; {1}; {2};{3};{4};{5}", stationName, day, timeband, i, p, weight[i]);
                            System.Console.Out.WriteLine(lined);

                            detail.Add(lined);
                        }

                    }

                    tbres.Add(string.Format("{0};{1};{2};{3}", stationName, day, timeband, effectif));


                    // Ecriture de tous les jours et 1/2 de la stations
                    // line = string.Format("{0}; {1}; {2};{3}", stationName, day, timeband, effectif);
                    // System.Console.Out.WriteLine(line);

                }

                double effectifday = 0;

                for (int i = 0; i < indivCount; i++)
                {
                    effectifday += dayprobas[i] * weight[i];
                }

                dres.Add(string.Format("{0};{1};;{2}", stationName, day, effectifday));

            }




        }


        public string[] days = new string[] { "LV", "SA", "DI" };

        public string[] timebands = new string[] {
            "05h00 - 05h30",
            "05h30 - 06h00",
            "06h00 - 06h30",
            "06h30 - 07h00",
            "07h00 - 07h30",
            "07h30 - 08h00",
            "08h00 - 08h30",
            "08h30 - 09h00",
            "09h00 - 09h30",
            "09h30 - 10h00",
            "10h00 - 10h30",
            "10h30 - 11h00",
            "11h00 - 11h30",
            "11h30 - 12h00",
            "12h00 - 12h30",
            "12h30 - 13h00",
            "13h00 - 13h30",
            "13h30 - 14h00",
            "14h00 - 14h30",
            "14h30 - 15h00",
            "15h00 - 15h30",
            "15h30 - 16h00",
            "16h00 - 16h30",
            "16h30 - 17h00",
            "17h00 - 17h30",
            "17h30 - 18h00",
            "18h00 - 18h30",
            "18h30 - 19h00",
            "19h00 - 19h30",
            "19h30 - 20h00",
            "20h00 - 20h30",
            "20h30 - 21h00",
            "21h00 - 21h30",
            "21h30 - 22h00",
            "22h00 - 22h30",
            "22h30 - 23h00",
            "23h00 - 23h30",
            "23h30 - 24h00",
            "24h00 - 24h30",
            "24h30 - 25h00",
            "25h00 - 25h30",
            "25h30 - 26h00",
            "26h00 - 26h30",
            "26h30 - 27h00",
            "27h00 - 27h30",
            "27h30 - 28h00",
            "28h00 - 28h30",
            "28h30 - 29h00",
             };

        private static void ProcessFortranFile(FortranProcessor fortranProcessor, string fortranFile, bool run)
        {
            fortranProcessor.ProcessFortanFile(fortranFile);
            fortranProcessor.CompileLinkAndRun(fortranFile, run);
        }


        private void CutSupFileIDF_NAT()
        {
            int entryCount = 24 * 2 * 3;

            int padCount = 2;
            int natCount_v1 = 36;
            int idfCount_v1 = 13;

            int oneSize = entryCount * this.IndivCount * 2;
            int padSize = 2 * this.IndivCount * 2;

            int toAddCount = 1;

            int toAddSize = toAddCount * oneSize;

            //int[] toReplaceIdxv1 = new int[] { 1 };
            //int[] toReplaceIdxv2 = new int[] { 1 };
            //string[] toReplaceName = new string[]
            //{
            //    "Les indés radios sans Sud+Wit"
            //};


            int[] toAddIdx = new int[] { 35 };
            string[] toAddName = new string[]
            {
                "Les Indés Capitale"
            };

            var outputpath = this.OutputPath.Substring(0, this.OutputPath.Length - 1);

            string outputv1 = outputpath + "_v1+v2+v3+v4";
            string outputv2 = outputpath;
            string outputv1v2 = outputpath + "_v1+v2+v3+v4+v5";

            var data_v1 = File.ReadAllBytes(Path.Combine(outputv1, "PANRA112.SUP"));
            var data_v2 = File.ReadAllBytes(Path.Combine(outputv2, "PANRA112.SUP"));

            int padnatSize = (natCount_v1 * oneSize) + padSize;

            int idfSize = (idfCount_v1) * oneSize;

            if (data_v1.Length != (padnatSize + idfSize))
                throw new Exception("file size error...");

            byte[] data_v1v2 = new byte[padnatSize + idfSize + toAddSize];

            // Copy PAD + NAT
            int totalSize = padnatSize;
            Array.Copy(data_v1, 0, data_v1v2, 0, padnatSize);

            // Copy IDF
            totalSize += idfSize;
            Array.Copy(data_v1, padnatSize, data_v1v2, padnatSize + toAddSize, idfSize);

            int added = 0;
            foreach (var addIdx in toAddIdx)
            {
                int srcPos = padSize + (addIdx * oneSize);

                // Insert to add
                totalSize += oneSize;
                Array.Copy(data_v2, srcPos, data_v1v2, padnatSize + (added * oneSize), oneSize);

                added++;
            }

            for (int i = 0; i < padnatSize; i++)
            {
                if (data_v1[i] != data_v1v2[i])
                    throw new Exception("Copy error");
            }

            //// Replace in NAT
            //int replaced = 0;
            //for (int i = 0; i < toReplaceIdxv2.Length; i++)
            //{
            //    int srcIdx = toReplaceIdxv2[i];
            //    int srcPos = padSize + (srcIdx * oneSize);

            //    int dstIdx = toReplaceIdxv1[i];
            //    int dstPos = padSize + (dstIdx * oneSize);

            //    Array.Copy(data_v2, srcPos, data_v1v2, dstPos, oneSize);

            //    replaced++;
            //}

            File.WriteAllBytes(Path.Combine(outputv1v2, "PANRA112.SUP"), data_v1v2);

            for (int i = 0; i < idfSize; i++)
            {
                if (data_v1[padnatSize + i] != data_v1v2[padnatSize + toAddSize + i])
                    throw new Exception("Copy error");
            }

            added = 0;
            foreach (var addIdx in toAddIdx)
            {
                int srcPos = padSize + (addIdx * oneSize);

                for (int i = 0; i < oneSize; i++)
                {
                    if (data_v2[srcPos + i] != data_v1v2[padnatSize + (added * oneSize) + i])
                        throw new Exception("Copy error");
                }
                added++;
            }

        }

        // Récupère path pour les fichiers (3) générés pas Kantar
        // soit
        // 1/ U1YY.sup.desc             ou  C1YY.sup.desc ou
        // 2/ U1YY.sup                  ou  C1YY.sup ou
        // 3/ U1YY_sta_exception.desc   ou  C1YY_sta_exception.desc
        // 4/ U1YY_Pop.txt              ou  C1YY_Pop.txt
        //
        public string GetPathYear(Enquete enquete, string inputPath, string libafterYear, string extension)
        {
            string pathReturn = "";
            int valYear = Convert.ToInt32(YearName);
            if (enquete == Enquete.PanelNational)
            {
                // Path National
                if (valYear <= 19)
                    pathReturn = Path.Combine(inputPath, "U" + YearName + "1" + libafterYear + "." + extension);
                else
                    pathReturn = Path.Combine(inputPath, "U1" + YearName + libafterYear + "." + extension);
            }
            else if (enquete == Enquete.PanelCadre)
            {
                // Path Cadre
                if (valYear <= 19)
                    pathReturn = Path.Combine(inputPath, "C" + YearName + "1" + libafterYear + "." + extension);
                else
                    pathReturn = Path.Combine(inputPath, "C1" + YearName + libafterYear + "." + extension);
            }
            else if (enquete == Enquete.PanelIleDeFrance)
            {
                // A priori même chemin que pour les Cadres mais "prudence !!!"
                // Path IDF
                if (valYear <= 19)
                    pathReturn = Path.Combine(inputPath, "C" + YearName + "1" + libafterYear + "." + extension);
                else
                    pathReturn = Path.Combine(inputPath, "C1" + YearName + libafterYear + "." + extension);
            }
            return pathReturn;
        }

        //private void CutSupFileIDF_NAT()
        //{
        //    int entryCount = 24 * 2 * 3;

        //    int padCount = 2;
        //    int natCount_v1 = 31;
        //    int idfCount_v1 = 13;

        //    int oneSize = entryCount * this.IndivCount * 2;
        //    int padSize = 2 * this.IndivCount * 2;

        //    int toAddCount = 3;

        //    int toAddSize = toAddCount * oneSize;

        //    int[] toAddIdx = new int[] { 30, 32, 33 };
        //    string[] toAddName = new string[]
        //    {
        //        "Lagardère Métropoles IDF", 
        //        "Lip !", 
        //        "Les Parisiennes"
        //    };

        //    var outputpath = this.OutputPath.Substring(0, this.OutputPath.Length - 1);

        //    string outputv1 = outputpath + "_v1+v2";
        //    string outputv2 = outputpath;
        //    string outputv1v2 = outputpath + "_v1+v2+v3";

        //    var data_v1 = File.ReadAllBytes(Path.Combine(outputv1, "PANRA112.SUP"));
        //    var data_v2 = File.ReadAllBytes(Path.Combine(outputv2, "PANRA112.SUP"));

        //    int padnatSize = (natCount_v1 * oneSize) + padSize;

        //    int idfSize = (idfCount_v1) * oneSize;

        //    if (data_v1.Length != (padnatSize + idfSize))
        //        throw new Exception("file size error...");

        //    byte[] data_v1v2 = new byte[padnatSize + idfSize + toAddSize];

        //    // Copy PAD + NAT
        //    int totalSize = padnatSize;
        //    Array.Copy(data_v1, 0, data_v1v2, 0, padnatSize);

        //    // Copy IDF
        //    totalSize += idfSize;
        //    Array.Copy(data_v1, padnatSize, data_v1v2, padnatSize + toAddSize, idfSize);

        //    int added = 0;
        //    foreach (var addIdx in toAddIdx)
        //    {
        //        int srcPos = padSize + (addIdx * oneSize);

        //        // Insert to add
        //        totalSize += oneSize;
        //        Array.Copy(data_v2, srcPos, data_v1v2, padnatSize + (added * oneSize), oneSize);

        //        added++;
        //    }


        //    File.WriteAllBytes(Path.Combine(outputv1v2, "PANRA112.SUP"), data_v1v2);


        //    for (int i = 0; i < padnatSize; i++)
        //    {
        //        if (data_v1[i] != data_v1v2[i])
        //            throw new Exception("Copy error");
        //    }

        //    for (int i = 0; i < idfSize; i++)
        //    {
        //        if (data_v1[padnatSize + i] != data_v1v2[padnatSize + toAddSize + i])
        //            throw new Exception("Copy error");
        //    }

        //    added = 0;
        //    foreach (var addIdx in toAddIdx)
        //    {
        //        int srcPos = padSize + (addIdx * oneSize);

        //        for (int i = 0; i < oneSize; i++)
        //        {
        //            if (data_v2[srcPos + i] != data_v1v2[padnatSize + (added * oneSize) + i])
        //                throw new Exception("Copy error");
        //        }
        //        added++;
        //    }

        //}

        public class Station
        {

            public Station(int id, string name)
            {
                Id = id;
                Name = name;
                Mode = eSignVariable.None;
                Index = 0;
                U1xxIdx = 0;
                IsIdf = false;
            }

            public Station(int id, string name, eSignVariable mode, int index)
            {
                Id = id;
                Name = name;
                Mode = mode;
                Index = index;
                U1xxIdx = 0;
                IsIdf = false;
            }

            public enum eSignVariable { None, Notoriety, Habit };

            public int Id { get; private set; }

            public string Name { get; private set; }

            public string Comment { get; set; }

            public eSignVariable Mode { get; set; }

            public int Index { get; set; }

            public bool IsIdf { get; set; }

            public int U1xxIdx { get; set; }

            public override string ToString()
            {
                return (IsIdf ? "IDF" : "NAT") + ";id=" + Id + "; Name=" + Name + ";idx=" + Index + ";uidx=" + U1xxIdx;
            }


        }

        public class SignVariable
        {

            public SignVariable(string type, int rep, int len, int pos, string comment)
            {
                Type = type;
                Repetition = rep;
                Length = len;
                Position = pos;
                Comment = comment;
            }

            public string Type { get; private set; }
            public int Repetition { get; private set; }
            public int Length { get; private set; }
            public int Position { get; set; }
            public string Comment { get; private set; }
        }

        [DllImport("kernel32.dll")]
        public static extern int GetShortPathName(string longPath, StringBuilder buffer, int bufferSize);

    }
}

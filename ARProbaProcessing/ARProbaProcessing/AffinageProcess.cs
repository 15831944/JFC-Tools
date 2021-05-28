using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ARProbaProcessing
{


    public partial class AffinageProcess
    {

        private int[] NOTE = new int[25 + 1] { 999999, 0, 12, 6, 4, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        private int[] NBIT = new int[] { 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        private int[] ITH = new int[] { 999999,
                9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
                1,1,1,1,
                2,2,2,2,2,2,2,2,2,2,2,2,
                3,3,3,3,3,3,3,3,3,3,3,3,
                4,4,4,4,4,4,4,4,
                5,5,5,5,5,5,5,5,
                6,6,6,6,6,6,6,6,
                7,7,7,7,7,7,7,7,
                8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};

        public void Run(ARProba arProba, Enquete enquete, string inputPath, string OutputPath)
        {
            #region BeginProcess
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
                var f04Processor = new F04Processor(arProba);
                f04Processor.ConvertBDEFiles();
            }
            ////////////////////////////////////////////////////////////

            var fortranProcessor = new FortranProcessor(arProba);
            FortranProcessor.VarHandlers varHandlers = new FortranProcessor.VarHandlers(arProba, fortranProcessor);

            // false : en mode Debug
            // true  : en mode Release
            bool run = false;
            if (!ModeDebug)
                run = true;
            #endregion EndProcess

            int year = 2000 + int.Parse(arProba.YearName);
            ContextPanel contextPanel;
            if (enquete == Enquete.PanelIleDeFrance)
            {
                contextPanel = new ContextPanel()
                {
                    Enquete = Enquete.PanelIleDeFrance,
                    IdxTot = 11,
                    NbSeg = 5,
                    TRegTest = 4,
                    SEGBASE = new int[] { 5, 8 },
                    SEG1 = new int[] { 999999, 1, 1, 2, 2, 3 },
                    SEG2 = new int[] { 999999, 1, 2, 2 },
                    SEG3 = null,
                    NIVO = new int[,] { {999999, 0,0,0,0,0 },
                                        {999999, 1,1,2,2,3 },
                                        {999999, 1,1,2,2,2 },
                                        {999999, 1,1,1,1,1} },
                    Year = year,
                    SIGN_LINE_LEN_BEFORE_HAB = SIGN_LINE_LEN_BEFORE_HAB_FCT(arProba),
                    SIGN_LINE_LEN_AFTER_HAB = SIGN_LINE_LEN_AFTER_HAB_WITH_PAD_FCT(arProba),
                    NB_STA_ALL_HAB = arProba.AllHabStationCount + arProba.HabAndNotoTotalStationListCount // a voir 1 = avant mise a zero de : arProba.HabAndNotoTotalStationListCount 
                };
            }
            else
            {
                contextPanel = new ContextPanel()
                {
                    Enquete = enquete,
                    IdxTot = 37,
                    NbSeg = 16,
                    TRegTest = 5,
                    SEGBASE = new int[] { 16, 26, 32 },
                    SEG1 = new int[] { 999999, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 9, 10, 10 },
                    SEG2 = new int[] { 999999, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6 },
                    SEG3 = new int[] { 999999, 1, 2, 2, 3, 3, 4 },
                    NIVO = new int[,]  { {0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
                                   {999999, 1,1,2,3,3,4,4,5,6,6,7,8,9,9,10,10 },
                                   {999999, 1,1,1,2,2,3,3,3,4,4,4,5,6,6,6,6},
                                   {999999, 1,1,1,2,2,2,2,2,3,3,3,3,4,4,4,4 },
                                   {999999, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } },
                    Year = year,
                    SIGN_LINE_LEN_BEFORE_HAB = SIGN_LINE_LEN_BEFORE_HAB_FCT(arProba),
                    SIGN_LINE_LEN_AFTER_HAB = SIGN_LINE_LEN_AFTER_HAB_FCT(arProba),
                    NB_STA_ALL_HAB = arProba.AllHabStationCount
            };
            }

            int NbStation = arProba.HabAndNotoStationCount;
            int NB_STA_HAB_NOTO = arProba.HabAndNotoStationCount;
            int NbGRPModulation = arProba.U1xxModalityCount + (enquete == Enquete.PanelIleDeFrance ? 0 : 2);
            int NbGRPStation = arProba.U1xxStationCount;

            string PathGRPWave = "";
            if (year <= 2019)
                PathGRPWave = Path.Combine(inputPath, EnqueteTools.GetSupFileChar(enquete) + (year % 100).ToString("00") + "1.SUP");
            else
                PathGRPWave = Path.Combine(inputPath, EnqueteTools.GetSupFileChar(enquete) + "1" + (year % 100).ToString("00") + ".SUP");

            List<int> lstPoids;
            List<int> lstAges;
            List<int> lstFiltreIDF;
            int nbJour = 23;
            //int IDXSUDRAD = 999;

            #region Definition colonnes
            contextPanel.COL_AGE3 = arProba.SignVars["AGE3"].Position - 1;
            contextPanel.COL_RUDA = arProba.SignVars["RUDA"].Position - 1;
            contextPanel.COL_PIAB = arProba.SignVars["PIAB"].Position - 1;
            contextPanel.COL_CSCI = arProba.SignVars["CSCI"].Position - 1;
            contextPanel.COL_SEX = arProba.SignVars["SEXE"].Position - 1;
            contextPanel.COL_AGE11 = arProba.SignVars["AG11"].Position - 1;
            contextPanel.COL_HAB = arProba.SignVars["HAB7"].Position - 1;
            contextPanel.COL_MENA = arProba.SignVars["MENA"].Position - 1;
            contextPanel.COL_RDA = arProba.SignVars["RDA"].Position - 1;
            contextPanel.COL_CSCC = arProba.SignVars["CSCC"].Position - 1;
            contextPanel.COL_NIEL = arProba.SignVars["NIEL"].Position - 1;
            contextPanel.COL_PG22 = arProba.SignVars["PG22"].Position - 1;
            contextPanel.COL_ENFA = arProba.SignVars["ENFA"].Position - 1;
            contextPanel.COL_ENF1 = arProba.SignVars["ENF1"].Position - 1;
            contextPanel.COL_ENF2 = arProba.SignVars["ENF2"].Position - 1;
            contextPanel.COL_ENF3 = arProba.SignVars["ENF3"].Position - 1;
            contextPanel.COL_ENF4 = arProba.SignVars["ENF4"].Position - 1;
            contextPanel.COL_NPER = arProba.SignVars["NPER"].Position - 1;
            contextPanel.COL_APRES = GetCOLAPRES(arProba, "ETVIE") - 1;
            contextPanel.COL_CELL = arProba.SignVars["CELL"].Position - 1;

            contextPanel.COL_TAB_CSCI = arProba.SignVars["CSCI"].Position;
            contextPanel.COL_TAB_SEX = arProba.SignVars["SEXE"].Position;
            contextPanel.COL_TAB_AGE11 = arProba.SignVars["AG11"].Position;
            contextPanel.COL_TAB_HAB = arProba.SignVars["HAB7"].Position;
            contextPanel.COL_TAB_MENA = arProba.SignVars["MENA"].Position;
            contextPanel.COL_TAB_RDA = arProba.SignVars["RDA"].Position;
            contextPanel.COL_TAB_CSCC = arProba.SignVars["CSCC"].Position;
            contextPanel.COL_TAB_NIEL = arProba.SignVars["NIEL"].Position;
            contextPanel.COL_TAB_PG22 = arProba.SignVars["PG22"].Position;
            contextPanel.COL_TAB_ENFA = arProba.SignVars["ENFA"].Position;
            contextPanel.COL_TAB_ENF1 = arProba.SignVars["ENF1"].Position;
            contextPanel.COL_TAB_ENF2 = arProba.SignVars["ENF2"].Position;
            contextPanel.COL_TAB_ENF3 = arProba.SignVars["ENF3"].Position;
            contextPanel.COL_TAB_ENF4 = arProba.SignVars["ENF4"].Position;
            contextPanel.COL_TAB_NPER = arProba.SignVars["NPER"].Position;
            contextPanel.COL_TAB_APRES = GetCOLAPRES(arProba, "ETVIE");
            contextPanel.COL_TAB_CELL = arProba.SignVars["CELL"].Position;
            contextPanel.COL_TAB_AGE3 = arProba.SignVars["AGE3"].Position;
            contextPanel.COL_TAB_RUDA = arProba.SignVars["RUDA"].Position;
            contextPanel.COL_TAB_PIAB = arProba.SignVars["PIAB"].Position;

            GetIndiceUXXX(PathGRPWave + ".desc", enquete, out contextPanel.IND_CSP, out contextPanel.IND_AGE, out contextPanel.IND_SEX, out contextPanel.IND_REG, 
                out contextPanel.Age35a59, out contextPanel.Age60Plus,
                out contextPanel.Age20a24, out contextPanel.Age25a34, out contextPanel.Age35a49, out contextPanel.Age50a64, out contextPanel.Age65Plus);

            #endregion Definition colonnes

            #region entrées lecpanel
            string pathSIGJFC_BDE = Path.Combine(inputPath, @"Bde\sig" + (year % 100).ToString("00") + "jfc.bde");
            lecpanel(contextPanel, pathSIGJFC_BDE, out lstPoids, out lstAges, out lstFiltreIDF);
            #endregion entrées lecpanel

            #region entrées regr5jp2
            string pathPansem = Path.Combine(OutputPath, "jfc");
            #endregion entrées regr5jp2

            #region entrées Fushab09

            int[] TABRH = HAB_STA_LIST_ID_NOTO_SET_TO_TOTAL_RADIO_FCT(arProba);
            string pathPANSIGN = Path.Combine(OutputPath, "PANSIGN");
            string pathSIGJFC2_BDE = Path.Combine(inputPath, @"Bde\sig" + (year % 100).ToString("00") + "jfc2.bde");
            #endregion

            #region entrées segpanel

            string pathSortie1 = Path.Combine(OutputPath, "SORTIE1.TXT");
            #endregion entrées segpanel

            #region entrées ecrpan1
            string pathF04 = Path.Combine(inputPath, "F04");
            string pathJNNIV = Path.Combine(OutputPath, "JFC");
            int nbStationTotal = arProba.StationCount;
            int nbStationHabNotoTotal = arProba.HabAndNotoStationCount + arProba.HabAndNotoTotalStationListCount;
            int nbStationHabNoto = arProba.HabAndNotoStationCount;


            int[] ITS = new int[nbStationHabNotoTotal + 1];
            int i = 1;
            foreach (var id in arProba.HabAndNotoStationList.Concat(arProba.HabAndNotoTotalStationList).Select(x => x.Id))
                ITS[i++] = id;
            #endregion entrées ecrpan1

            #region entrées ecrsegpa
            int SIGN_LINE_LEN_FULL = arProba.SigLineLen + 2;
            string pathSortie5 = Path.Combine(OutputPath, "SORTIE5.TXT");
            string pathSegs = Path.Combine(OutputPath, "POIDSEGS");
            #endregion entrées ecrsegpa

            #region entrées calcnivo
            string pathNiveaux = Path.Combine(OutputPath, "NIVEAUX");
            #endregion entrées calcnivo

            #region entrées crenonin
            string pathNinities = Path.Combine(OutputPath, "NINITIES");
            List<Tuple<int, int>> stationApres = CRENONIN_NOTO_HANDLING(arProba);
            #endregion entrées crenonin

            #region entrées sav1qh..
            string pathSortiesav1qhpa = Path.Combine(OutputPath, "SORTIE.sem");
            string pathSortiesav1qhps = Path.Combine(OutputPath, "SORTIE.sam");
            string pathSortiesav1qhpd = Path.Combine(OutputPath, "SORTIE.dim");

            string pathZuptause = Path.Combine(OutputPath, "ZUPTAUSE");
            string pathZuptausa = Path.Combine(OutputPath, "ZUPTAUSA");
            string pathZuptaudi = Path.Combine(OutputPath, "ZUPTAUDI");
            #endregion entrées sav1qh..

            #region entrées cgrp75b
            string pathSortie8 = Path.Combine(OutputPath, "SORTIE8.TXT");
            string pathNOUVOGRP = Path.Combine(OutputPath, "NOUVOGRP.TXT");
            int[] ISTAcgrp75br = HAB_NOTO_STA_LIST_U1XX_INDEXES(arProba);
            #endregion entrées cgrp75b

            #region entrées chab1qhp
            string pathHab = Path.Combine(OutputPath, "habqhind");
            int[] ISTA = HAB_0_NOTO_1_STA_LIST_MASK(arProba);
            #endregion entrées chab1qhp

            #region entrées caud1qhp
            string pathAudQhInd = Path.Combine(OutputPath, "AudQhInd");
            #endregion entrées caud1qhp

            #region entrées calcreg
            string pathCellule = Path.Combine(OutputPath, "Cellules");
            #endregion entrées calcreg

            #region entrées caudtotp
            string pathNoteIndiv = Path.Combine(OutputPath, "NOTINDIV");

            #endregion entrées caudtotp

            #region entrées cont75br
            int popLV = int.Parse(arProba.U1xxPopTxt[0]);
            int popS = int.Parse(arProba.U1xxPopTxt[1]);
            int popD = int.Parse(arProba.U1xxPopTxt[2]);
            string pathSortie9 = Path.Combine(OutputPath, "SORTIE9.TXT");
            #endregion entrées cont75br

            #region entrées cnzuptse
            string pathCnzuptse = Path.Combine(OutputPath, "SORTIESE.COR");
            string pathCnzuptsa = Path.Combine(OutputPath, "SORTIESA.COR");
            string pathCnzuptdi = Path.Combine(OutputPath, "SORTIEDI.COR");
            string pathzuptauseCor = Path.Combine(OutputPath, "ZUPTAUSE.COR");
            string pathzuptausaCor = Path.Combine(OutputPath, "ZUPTAUSA.COR");
            string pathzuptaudiCor = Path.Combine(OutputPath, "ZUPTAUDI.COR");
            #endregion entrées cnzuptse..

            #region entrées attribp2
            string pathAttribp2 = Path.Combine(OutputPath, "SORTIE10.TXT");
            string pathPanecr = Path.Combine(OutputPath, "PANECR" + (year % 100).ToString("00"));
            #endregion entrées attribp2

            #region entrées Transp08
            int NB_STA_IDF = arProba.HabAndNotoStationList.Count(x => x.IsIdf);
            string pathTransp08 = Path.Combine(OutputPath, "2020.SUP");
            string pathYearNat = Path.Combine(OutputPath, "PANRA1" + (year % 100).ToString("00") + (enquete == Enquete.PanelIleDeFrance ? ".SUP" : ".NAT"));
            string pathYearIdf = Path.Combine(OutputPath, "PANRA1" + (year % 100).ToString("00") + ".IDF");
            string pathYearSup = Path.Combine(OutputPath, "PANRA1" + (year % 100).ToString("00") + ".SUP");
            int[] STA_IDF_LIST_NO_SUDRAD_MASK = GET_STA_IDF_LIST_NO_SUDRAD_MASK(arProba);
            #endregion entrées Transp08

            #region entrées crecib08
            string pathPan20Cib = Path.Combine(OutputPath, "PAN" + (year % 100).ToString("00") + "CIB");
            #endregion entrées crecib08

            #region entrées penetr
            string pathPenetr = Path.Combine(OutputPath, "penetr");
            List<string> strStations = arProba.HabAndNotoStationList.Select(x => x.Name).ToList();
            strStations.AddRange(arProba.HabAndNotoTotalStationList.Select(x => x.Name).ToList());
            #endregion entrées penetr

            #region entrées Asymp
            string pathAS5H5H = Path.Combine(OutputPath, "AS5H5H");
            string headerAS5H5H = "             PANEL MEDIAMETRIE RADIO " + year.ToString() +
                Environment.NewLine +
                " PENETRATIONS CUMULEES MAXIMALES 5H-29H LUNDI-DIMANCHE " +
                Environment.NewLine + Environment.NewLine + Environment.NewLine +
                " STATIONS/AGREGATS       ENSEMBLE     MENAGERES       13-28";
            // TODO: différence entre le fichier template quer j'ai avec la sortie du fichier donné par Fred
            // Template : ...."STATIONS/AGREGATS      ENSEMBLE  MENAGERES    13-28'-/ 24X,'13 ANS +   - 50 ANS'
            #endregion entrées Asymp


            int NBINDIV = segpanel(contextPanel, pathSIGJFC_BDE, pathSortie1);

            //NbStation = arProba.HabAndNotoTotalStationListCount;
            VsorPoid[][] JN = ecrpan1j(contextPanel, pathF04, pathJNNIV, NBINDIV, nbStationHabNotoTotal, nbStationTotal, ITS, year);   // [Jour 1..23][Individus 1..N] = {VOSR[,]?, Poid[]}

            VsorPoid[][][] JNByWeek = regr5jp2(nbStationHabNotoTotal, NBINDIV, JN, pathPansem); // [Semaine 1..3][Jour de semaine Lundi à vendredi 1..5][indiv]

            List<Fushab09Indiv> fushab09Indivs = Fushab09(contextPanel, NB_STA_HAB_NOTO, TABRH, pathSIGJFC2_BDE, pathPANSIGN);

            int[][][][] NINI_IND_QH_W = chab1qhp(NB_STA_HAB_NOTO, fushab09Indivs, ISTA, pathHab); // Habitude [STATIONS, QH, LV/Sa/Di, INDIV];

            int[,] NINI_IND_STA = crenonin(contextPanel, nbJour, NB_STA_HAB_NOTO, fushab09Indivs, JN, stationApres, pathNinities); // [INDIV, STATIONS] 

            ecrsegpa(contextPanel, pathSIGJFC_BDE, pathSegs,NbStation, NBINDIV, SIGN_LINE_LEN_FULL, pathSortie5,
                     out int[,] POIDSEGM, out int IPOP);

            short[,,,] cellules = calcregr(contextPanel, NBINDIV, NB_STA_HAB_NOTO, NINI_IND_STA, POIDSEGM, JN, pathCellule); // int[LV/Sa/Di, STATIONS, QH, CELL];

            byte[,,,] regrs = calcnivo(contextPanel, NBINDIV, NB_STA_HAB_NOTO, cellules, pathNiveaux); // [LV/Sa/Di, STATIONS, QH, CELL]

            byte[][][][] audiences = caud1qhp(NBINDIV, NB_STA_HAB_NOTO, JN, POIDSEGM, pathAudQhInd); // audiences[STATIONS, INdiv, QH, 1..3]

            float[] noteIndiv = caudtotp(contextPanel, NBINDIV, NB_STA_HAB_NOTO, JN, POIDSEGM, fushab09Indivs, pathNoteIndiv);

            float[,,,] ZUPTAUSE = sav1qhpa(contextPanel, NBINDIV, NB_STA_HAB_NOTO, regrs, POIDSEGM, fushab09Indivs, JNByWeek, JN, pathZuptause, pathSortiesav1qhpa); // [STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL];

            float[,,,] ZUPTAUSA = sav1qhps(contextPanel, NBINDIV, NB_STA_HAB_NOTO, regrs, POIDSEGM, fushab09Indivs, JNByWeek, JN, pathZuptausa, pathSortiesav1qhps); // [STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL];

            float[,,,] ZUPTAUDI = sav1qhpd(contextPanel, NBINDIV, NB_STA_HAB_NOTO, regrs, POIDSEGM, fushab09Indivs, JNByWeek, JN, pathZuptaudi, pathSortiesav1qhpd); // [STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL];

            float[,,,] Couverture = (enquete == Enquete.PanelIleDeFrance) ?
                cgrp75br_Idf(contextPanel, PathGRPWave, NbStation, NbGRPModulation, NbGRPStation, ISTAcgrp75br, pathSortie8, pathNOUVOGRP) : // [LV/Sa/Di, QH, 4 + 1, CELL];
                cgrp75br(contextPanel, PathGRPWave, NbStation, NbGRPModulation, NbGRPStation, ISTAcgrp75br, pathSortie8, pathNOUVOGRP);  // [LV/Sa/Di, QH, 4 + 1, CELL];

            cont75br(contextPanel, NBINDIV, NB_STA_HAB_NOTO, popLV, popS, popD, Couverture, pathSortie9);

            float[,,,] ZUPTAUSECOR = cnzuptse(contextPanel, NBINDIV, NB_STA_HAB_NOTO, pathCnzuptse, pathzuptauseCor, Couverture, regrs, ZUPTAUSE);  // float[STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL];

            float[,,,] ZUPTAUSACOR = cnzuptsa(contextPanel, NBINDIV, NB_STA_HAB_NOTO, pathCnzuptsa, pathzuptausaCor, Couverture, regrs, ZUPTAUSA);

            float[,,,] ZUPTAUDICOR = cnzuptdi(contextPanel, NBINDIV, NB_STA_HAB_NOTO, pathCnzuptdi, pathzuptaudiCor, Couverture, regrs, ZUPTAUDI);

            byte[][][][] PROBAS = attribp2(contextPanel, NBINDIV, NB_STA_HAB_NOTO, regrs, POIDSEGM, NINI_IND_STA, noteIndiv, audiences, NINI_IND_QH_W,
                ZUPTAUSECOR, ZUPTAUSACOR, ZUPTAUDICOR, pathAttribp2, pathPanecr); // [STATIONS, LV/Sa/Di, QH, INDIVS]

            ZUPTAUSECOR = ZUPTAUSECOR = ZUPTAUDICOR = Couverture = ZUPTAUSE = ZUPTAUSA = ZUPTAUDI = null;
            regrs = null;
            audiences = null;
            cellules = null;
            JNByWeek = null;
            NINI_IND_QH_W = null;
            NINI_IND_STA = null;
            GC.Collect();

            BSupport BSUP = transp08(contextPanel, NBINDIV, NB_STA_HAB_NOTO, NB_STA_IDF, STA_IDF_LIST_NO_SUDRAD_MASK, POIDSEGM, lstFiltreIDF, lstPoids, PROBAS, pathTransp08, pathYearNat, pathYearIdf, pathYearSup);

            int[,] PANCIB = (enquete == Enquete.PanelIleDeFrance) ?
                crecib08_Idf(contextPanel, NBINDIV, fushab09Indivs, pathPan20Cib) :
                crecib08(contextPanel, NBINDIV, fushab09Indivs, pathPan20Cib); // [3 + 1, NIND + 1]

            penetr(NBINDIV, nbStationHabNotoTotal, JN, lstPoids, pathPenetr, int.Parse(arProba.U1xxPopTxt[0]), strStations);

            asympt(contextPanel, NBINDIV, NB_STA_HAB_NOTO, nbStationHabNotoTotal, BSUP, PANCIB, pathAS5H5H, strStations);

            #region Endprocess

            // Make station list
            StringBuilder staList = new StringBuilder();
            staList.AppendLine("=====Nat=======");
            foreach (var station in arProba.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO"))
                {
                    staList.AppendLine(station.Name + "; " + station.Comment);
                }
            }
            staList.AppendLine("=====IDF=======");
            foreach (var station in arProba.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO") && station.IsIdf)
                {
                    staList.AppendLine(station.Name + "; " + station.Comment);
                }
            }

            File.WriteAllText(OutputPath + @"\StationList.txt", staList.ToString());
            #endregion Endprocess
        }

        private void lecpanel(ContextPanel contextPanel, string Path_SIGJFC_BDE, out List<int> lstPoids, out List<int> lstAges, out List<int> lstFiltreIDF)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CONTROLE DES POIDS DE REDRESSEMENT
            // SORTIE D UN FICHIER AGE NECESSAIRE A LA FABRICATION DU FICHIER POUR MEDIAPOLIS
            //
            //INCLUDE 'FSUBLIB.FI'
            //COMMON / NUBIT / NBIT
            //SAVE / NUBIT /

            int IPERS, IAGE, ICL, COMPTIDF;
            // Attention, le nombre de caractÃ¨res rÃ©el est de 599 colonnes(on en rajoute 2 pour le retour Chariot)

            //                  INITIALISATIONS
            int IG = 0;
            int I15 = 0;
            int IPOP = 0;
            COMPTIDF = 0;

            //         OUVERTURE FICHIERS
            //WRITE(*, *) 'Hello World!'
            Console.WriteLine("Hello World!");

            //OPEN(13, FILE = '#SIGJFC_BDE#',
            //    -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            IEnumerable<string> KHI2 = File.ReadLines(Path_SIGJFC_BDE);

            //OPEN(14, FILE = '#OUTPUT#POIDS',
            //    -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')

            lstPoids = new List<int>();


            //OPEN(15, FILE = '#OUTPUT#AGES',
            //    -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')

            lstAges = new List<int>();

            //OPEN(16, FILE = '#OUTPUT#FILTREIF',
            //    -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')

            lstFiltreIDF = new List<int>();

            // BOUCLE INDIVIDUS
            foreach (string strIndiv in KHI2)
            {
                IAGE = 1;
                if (strIndiv[contextPanel.COL_AGE3] == '1')
                    IAGE = 2;

                // Filtre REGION PARISIENNE(Région Uda)
                ICL = 0;
                if (strIndiv[contextPanel.COL_RUDA] == '1')
                {
                    ICL = 1;
                    COMPTIDF = COMPTIDF + 1;
                }

                IPERS = int.Parse(strIndiv.Substring(contextPanel.COL_PIAB, 5));
                if (IPERS <= 0) continue;
                if (IPERS > 3276) Console.WriteLine($"IPERS= {IPERS}");

                // On comptabilise les individus
                IG = IG + 1;
                IPOP = IPOP + IPERS * 10;

                lstPoids.Add(IPERS);
                lstAges.Add(IAGE);
                lstFiltreIDF.Add(ICL);
            }

            //120 WRITE(6, 1) IG, IPOP, COMPTIDF
            Console.WriteLine("NbIndiv : " + IG + " Population : " + IPOP + " NbIndivIDF : " + COMPTIDF);
        }

        private int segpanel(ContextPanel contextPanel, string pathSIGJFC_BDE, string pathSortie1)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CONTROLE DES VOLUMES DES SEGMENTS
            //
            // Longueur d'une ligne: 599 + 2 car de fin
            int[] SEGM = new int[16 + 1];
            int ICSP, IREG, ISEX, IAGE, ISEG, IPERS, AGE;

            // Nous avons besoin de la variable AGE pour le calcul de l'age qui est sur deux colonnes

            int IG = 0;
            int IPOP = 0;
            IEnumerable<string> KHI2 = File.ReadLines(pathSIGJFC_BDE);
            foreach (string strIndiv in KHI2)
            {
                // CALCUL DU POIDS(Colonnes 9 à 13 inclues)
                IPERS = int.Parse(strIndiv.Substring(contextPanel.COL_PIAB, 5));

                if (IPERS <= 0) continue;
                IG++;
                IPOP += IPERS * 10;

                //  DETERMINATION DU SEGMENT
                //  ------------------ -
                //   CSP de l'interviewé
                //  ------------------ -
                //   La CSP de l'individu est codé sur une colonne (colonne 18)
                //   ICSP = 1 correspond aux CSP +
                //   ICSP = 2 correspond aux CSP -
                //   ICSP = 3 correspond aux inactifs

                ICSP = 1;
                if ((strIndiv[contextPanel.COL_CSCI] == '1') || (int.Parse(strIndiv[contextPanel.COL_CSCI].ToString()) > 4)) ICSP = 2;
                if (int.Parse(strIndiv[contextPanel.COL_CSCI].ToString()) > 6) ICSP = 3;

                //  --------------------
                //   Sexe de l'interviewé
                //  --------------------
                //   Le sexe de l'interviewé est codé sur une colonne (colonne 14)
                //   ISEX = 1 correspond aux Hommes
                //   ISEX = 2 correspond aux Femmes

                ISEX = int.Parse(strIndiv[contextPanel.COL_SEX].ToString());

                //  ----------------- -
                //   Age de l'interviewé
                //  ------------------ -
                //  L'Age de l'interviewé est codé sur deux colonnes(colonne 24 et 25)
                //  IAGE = 1 correspond aux 13 - 34 ans
                //  IAGE = 2 correspond aux 35 - 59 ans
                //  IAGE = 3 correspond aux 60 ans et +
                AGE = int.Parse(strIndiv.Substring(contextPanel.COL_AGE11, 2));
                IAGE = 1;
                if (contextPanel.Age35a59.Contains(AGE)) IAGE = 2;
                if (contextPanel.Age60Plus.Contains(AGE)) IAGE = 3;

                //  --------------------------
                //   Région UDA de l'interviewé
                //  --------------------------
                //   La région UDA de l'interviewé est codé sur une colonne (colonne 20)
                //   IREG = 1 correspond aux 6 UDA Nord
                //   IREG = 2 correspond aux 3 UDA Sud
                IREG = 1;
                if (int.Parse(strIndiv[contextPanel.COL_RUDA].ToString()) > 6) IREG = 2;

                //C-------------------------------------------------------------------------- -
                //  C On fonction des variables précédemment calculées, on détermine les segments
                //  C-------------------------------------------------------------------------- -
                if (contextPanel.Enquete != Enquete.PanelIleDeFrance)
                {
                    if (IAGE != 3)
                    {
                        if (ICSP == 1)
                        {
                            ISEG = 3;
                            if (IREG == 1 && ISEX == 1) ISEG = 1;
                            if (IREG == 1 && ISEX == 2) ISEG = 2;
                        }
                        else
                        {
                            if (ICSP != 3)
                            {
                                if (IAGE != 2)
                                {
                                    ISEG = 4;
                                    if (ISEX == 2) ISEG = 5;
                                }
                                else
                                {
                                    ISEG = 11;
                                    if (IREG == 1 && ISEX == 1) ISEG = 9;
                                    if (IREG == 1 && ISEX == 2) ISEG = 10;
                                }
                            }
                            else
                            {
                                if (IAGE != 2)
                                {
                                    ISEG = 8;
                                    if (IREG == 1 && ISEX == 1) ISEG = 6;
                                    if (IREG == 1 && ISEX == 2) ISEG = 7;
                                }
                                else
                                {
                                    ISEG = 12;
                                }
                            }
                        }
                    }
                    else
                    {
                        ISEG = 13;
                        if (IREG == 2) ISEG = 14;
                        if (ISEX == 2) ISEG = ISEG + 2;
                    }

                    if (ISEG < 1 || ISEG > 16)
                    {
                        Console.WriteLine($" ISEG= {ISEG}");
                        return -1;
                    }
                    else
                    {
                        SEGM[ISEG]++;
                    }
                }
                else if (contextPanel.Enquete == Enquete.PanelIleDeFrance)
                {
                    // Segment 1 = Hommes de 13 à 34 ans
                    if ((ISEX == 1) && (IAGE == 1)) SEGM[1] += 1;
                    // Segment 2 = Femmes de 13 à 34 ans
                    if ((ISEX == 2) && (IAGE == 1)) SEGM[2] += 1;
                    // Segment 3 = Hommes de 35 à 59 ans
                    if ((ISEX == 1) && (IAGE == 2)) SEGM[3] += 1;
                    // Segment 4 = Femmes de 35 à 59 ans
                    if ((ISEX == 2) && (IAGE == 2)) SEGM[4] += 1;
                    // Segment 5 = 60 ans et +
                    if (IAGE == 3) SEGM[5] += 1;
                }
            } // foreach (string strIndiv in KHI2)

            if (File.Exists(pathSortie1)) File.Delete(pathSortie1);
            StreamWriter swSortie1 = new StreamWriter(File.Create(pathSortie1));
            swSortie1.WriteLine($"NBGUS: {IG}   POPULATION: {IPOP}");

            for (int I = 1; I <= contextPanel.NbSeg; I++)
            {
                float PEN = SEGM[I];
                PEN = PEN * 100f / IG;
                swSortie1.WriteLine($"CLASSE: {I.ToString("00")}   VOLUME : {SEGM[I]}   POURCENTAGE: {PEN.ToString("0.00")} ");
            }
            swSortie1.Close();

            return IG;
        }

        private VsorPoid[][] ecrpan1j(ContextPanel contextPanel, string pathF04, string pathNiv, int NBIND, int NBSTA, int NB_STA_TOTAL, int[] ITS, int year)
        {
            VsorPoid[][] resultJn = new VsorPoid[23 + 1][];
            for (int sor = 1; sor <= 23; sor++)
                resultJn[sor] = new VsorPoid[NBIND + 1];

            // PANEL RADIO 08 MEDIAMETRIE(Nouveau format)
            // CREATION DU POIDS DES 1 / 4H POUR 1 JOUR

            //  Attention: ne pas oublier de changer NBSTA avec le nombre de stations utiliser dans hab et noto
            //PARAMETER(NBSTA =#NB_STA_HAB_NOTO_TOTAL#)

            int[] KHI2 = new int[3 + 1];

            // Attention: ne pas oublier de changer VRAD avec le nombre de stations totals

            int[] KDEB = new int[130 + 1];
            ushort[,] VRAD = new ushort[6 + 1, NB_STA_TOTAL + 1];
            int[] ITAP = new int[96 + 1];

            // INITIALISATIONS
            int IG;

            // OUVERTURE FICHIERS
            for (int IJ = 1; IJ <= 23; IJ++)
            {
                if (!Directory.Exists(pathNiv)) Directory.CreateDirectory(pathNiv);
                FileStream writeStream = new FileStream(Path.Combine(pathNiv, "JN" + IJ.ToString("00") + "NIV.JFC"), FileMode.Create);
                BinaryWriter writeBinaryJNOut = new BinaryWriter(writeStream);

                IG = 0;

                string pathJN = Path.Combine(pathF04, "JN" + IJ.ToString("00") + "NI" + (year % 100).ToString("00") + ".F04");
                FileInfo fi = new FileInfo(pathJN);
                if (fi.Length != 130 * 2 + NBIND * (3 * 2 + NB_STA_TOTAL * 6 * 2))
                {
                    throw new Exception($"ecrpan1j : taille du fichier d'entree JN incorrecte NBIND {NBIND} NB_STA_TOTAL {NB_STA_TOTAL}");
                };

                FileStream fs = File.Open(pathJN, FileMode.Open);
                fs.Seek(0, SeekOrigin.Begin);
                BinaryReader br = new BinaryReader(fs);

                // Lecture KDEB
                for (int i = 1; i <= 130; i++)
                    KDEB[i] = br.ReadUInt16();

                // BOUCLE INDIVIDUS
                while (fs.Position != fs.Length)
                {
                    for (int i = 1; i <= 3; i++)
                        KHI2[i] = br.ReadInt16();

                    for (int j = 1; j <= NB_STA_TOTAL; j++)
                        for (int i = 1; i <= 6; i++)
                            VRAD[i, j] = br.ReadUInt16();

                    if (KHI2[1] == 1)
                    {
                        IG = IG + 1;
                        for (int I = 1; I <= 96; I++) ITAP[I] = 0;

                        ushort[,] VSOR = new ushort[6 + 1, NBSTA + 1];
                        for (int IS = 1; IS <= NBSTA; IS++)
                        {
                            for (int I = 1; I <= 6; I++)
                            {
                                VSOR[I, IS] = VRAD[I, ITS[IS]];
                            }
                        }

                        for (int IS = 1; IS <= NBSTA - (contextPanel.Enquete != Enquete.PanelIleDeFrance ? 2 : 0); IS++)
                        {
                            //int IC = ITS[IS];
                            for (int QH = 1; QH <= 96; QH++)
                            {
                                ushort[] bits = new ushort[7];
                                for (int b = 1; b <= 6; b++)
                                    bits[b] = VSOR[b, IS];
                                if (L1BITFCT(bits, QH)) ITAP[QH] = ITAP[QH] + 1;
                            }
                        }

                        int[] POID = new int[96 + 1];
                        for (int I = 1; I <= 96; I++)
                        {
                            POID[I] = ITAP[I] + 1 < NOTE.Length ? NOTE[ITAP[I] + 1] : 1;
                        }

                        resultJn[IJ][IG] = new VsorPoid(NBSTA) { Poid = POID, VSor = VSOR };

                        for (int IS = 1; IS <= NBSTA; IS++)
                        {
                            for (int I = 1; I <= 6; I++)
                            {
                                writeBinaryJNOut.Write(Convert.ToUInt16(VSOR[I, IS]));
                            }
                        }

                        for (int I = 1; I <= 96; I++)
                        {
                            writeBinaryJNOut.Write(Convert.ToUInt16(POID[I]));
                        }

                    }
                    else
                    {
                        break;
                    }
                }

                br.Close();

                writeBinaryJNOut.Close();
                writeStream.Close();

                // FIN DE FICHIER
                Console.WriteLine(IJ.ToString() + " " + IG.ToString());
            }

            // ???? Console.WriteLine($"JOUR : {I2}   NBGUS : {I6});
            return resultJn;
        }


        private VsorPoid[][][] regr5jp2(int NBSTA, int nbIndiv, VsorPoid[][] JN, string pathPansem)
        {
            VsorPoid[][][] JNByWeek = new VsorPoid[3 + 1][][];   // [Semaine 1..3][Jour de semaine Lundi à vendredi 1..5][indiv]

            for (int week = 1; week <= 3; week++)
            {
                JNByWeek[week] = new VsorPoid[5 + 1][];
                for (int j = 1; j <= 5; j++)
                {
                    JNByWeek[week][j] = new VsorPoid[nbIndiv + 1];

                    for (int indiv = 1; indiv <= nbIndiv; indiv++)
                    {
                        int noJour = 2 + (week - 1) * 7 + j;
                        JNByWeek[week][j][indiv] = JN[noJour][indiv];                     // JN[Jour 1..23][Individus 1..N] = {VOSR[,]?, Poid[]}
                    }
                }
                FileStream writeStream = new FileStream(Path.Combine(pathPansem, "PANSEM" + week.ToString("0") + ".JFC"), FileMode.Create);
                BinaryWriter writeBinay = new BinaryWriter(writeStream);
                for (int IG = 1; IG <= nbIndiv; IG++)
                {
                    for (int J = 1; J <= 5; J++)
                    {
                        for (int IS = 1; IS <= NBSTA; IS++)
                        {
                            for (int I = 1; I <= 6; I++)
                            {
                                writeBinay.Write(JNByWeek[week][J][IG].VSor[I, IS]); // [Semaine 1..3] [Jour de semaine Lundi à vendredi 1..5] [indiv]
                            }

                        }
                        for (int QH = 1; QH <= 96; QH++)
                        {
                            writeBinay.Write(Convert.ToUInt16(JNByWeek[week][J][IG].Poid[QH]));
                        }
                    }
                }
                writeBinay.Close();
                writeStream.Close();
            }

            return JNByWeek;
        }

        public List<Fushab09Indiv> Fushab09(
            ContextPanel contextPanel,
            int NBSTA,
            int[] TABRH,
            string pathSIGJFC_BDE,
            string pathPANSIGN)
        {
            // C PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // C fusion des Habitudes total RADIO oubliees dans SIGN2007.F04
            // C        et rangement des habitudes dans le meme ordre que les audiences
            // C        avec traitement des 'sans habitudes'.
            // C
            // C
            // INCLUDE 'FSUBLIB.FI'
            // C Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)
            // PARAMETER (NBSTA=#NB_STA_HAB_NOTO#)

            // C Attention AVANT[#SIGN_LINE_LEN_BEFORE_HAB#) et le buffer de lecture pour se caler au début des données de HAB
            // C Attention APRES[#SIGN_LINE_LEN_AFTER_HAB#) et le buffer de lecture pour se caler à la fin de la ligne des données

            // INTEGER * 1 AVANT[#SIGN_LINE_LEN_BEFORE_HAB#),KHAB(9,#NB_STA_ALL_HAB#),KHSA(9,#NB_STA_ALL_HAB#),KHDI(9,#NB_STA_ALL_HAB#),APRES[#SIGN_LINE_LEN_AFTER_HAB#),CHARIOT(2)
            // INTEGER * 1 KHA2(9, NBSTA),KHS2(9, NBSTA),KHD2(9, NBSTA)

            // => EN PARAMETRE
            // INTEGER * 2 TABRH(NBSTA)
            // C Attention pour les stations avec Notoriété il faut metre les données total radio(#TOTAL_RADIO_INDEX#)
            // int[] TABRH = new int[NBSTA + 1]; 
            // int[] TABRH /#HAB_STA_LIST_ID_NOTO_SET_TO_TOTAL_RADIO#/

            // INTEGER*1 int KHAB(9,NB_STA_ALL_HAB),KHSA(9,#NB_STA_ALL_HAB#),KHDI(9,#NB_STA_ALL_HAB#),APRES[#SIGN_LINE_LEN_AFTER_HAB#), CHARIOT(2)
            byte[,] KHAB = new byte[9 + 1, contextPanel.NB_STA_ALL_HAB + 2];
            byte[,] KHSA = new byte[9 + 1, contextPanel.NB_STA_ALL_HAB + 2];
            byte[,] KHDI = new byte[9 + 1, contextPanel.NB_STA_ALL_HAB + 2];

            // INITIALISATIONS
            int G = 0;

            // OUVERTURE FICHIERS
            Console.WriteLine("NBSTA=" + NBSTA);
            Console.WriteLine("SIGN_LINE_LEN_BEFORE_HAB=" + contextPanel.SIGN_LINE_LEN_BEFORE_HAB);
            Console.WriteLine("SIGN_LINE_LEN_AFTER_HAB=" + contextPanel.SIGN_LINE_LEN_AFTER_HAB);
            Console.WriteLine("NB_STA_ALL_HAB=" + contextPanel.NB_STA_ALL_HAB);

            //
            //                              OUVERTURE FICHIER
            //
            // OPEN(13, FILE = '#SIGJFC_BDE#', -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')
            FileStream fs = File.Open(pathSIGJFC_BDE, FileMode.Open);
            fs.Seek(0, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fs);

            int IG = 0;
            List<Fushab09Indiv> fushab09Indivs = new List<Fushab09Indiv>();

            // BOUCLE INDIVIDUS
            while (fs.Position != fs.Length)
            {
                byte[] AVANT = new byte[contextPanel.SIGN_LINE_LEN_BEFORE_HAB + 1];
                byte[] APRES = new byte[contextPanel.SIGN_LINE_LEN_AFTER_HAB + 1];
                byte[] CHARIOT = new byte[2 + 1];

                // 30 READ(13, END = 120) AVANT,((KHAB(I, J), I = 1, 9), J = 1,#NB_STA_ALL_HAB#),((KHSA(I,J),I=1,9),J=1,#NB_STA_ALL_HAB#),
                // -((KHDI(I,J),I=1,9),J=1,#NB_STA_ALL_HAB#),APRES,CHARIOT

                for (int i = 1; i <= contextPanel.SIGN_LINE_LEN_BEFORE_HAB; i++)
                    AVANT[i] = br.ReadByte();

                for (int j = 1; j <= contextPanel.NB_STA_ALL_HAB; j++)
                    for (int i = 1; i <= 9; i++)
                        KHAB[i, j] = br.ReadByte();

                for (int j = 1; j <= contextPanel.NB_STA_ALL_HAB; j++)
                    for (int i = 1; i <= 9; i++)
                        KHSA[i, j] = br.ReadByte();

                for (int j = 1; j <= contextPanel.NB_STA_ALL_HAB; j++)
                    for (int i = 1; i <= 9; i++)
                        KHDI[i, j] = br.ReadByte();

                for (int i = 1; i <= contextPanel.SIGN_LINE_LEN_AFTER_HAB; i++)
                    APRES[i] = br.ReadByte();

                for (int i = 1; i <= 2; i++)
                    CHARIOT[i] = br.ReadByte();

                IG = IG + 1;

                // BOUCLE STATIONS
                byte[,] KHA2 = new byte[9 + 1, NBSTA + 1];
                byte[,] KHS2 = new byte[9 + 1, NBSTA + 1];
                byte[,] KHD2 = new byte[9 + 1, NBSTA + 1];
                for (int NOP = 1; NOP <= NBSTA; NOP++)
                {
                    int IP = TABRH[NOP];
                    for (int I = 1; I <= 9; I++)
                    {
                        KHA2[I, NOP] = KHAB[I, IP];
                        KHS2[I, NOP] = KHSA[I, IP];
                        KHD2[I, NOP] = KHDI[I, IP];
                    }
                }
                fushab09Indivs.Add(new Fushab09Indiv()
                {
                    AVANT = AVANT,
                    KHAB = KHA2,
                    KHSA = KHS2,
                    KHDI = KHD2,
                    APRES = APRES,
                    CHARIOT = CHARIOT,
                });
            }
            br.Close();

            // FIN DE FICHIER
            Console.WriteLine("NB.GUS=" + IG.ToString());

            // Ecriture Pansign
            FileStream writeStream = new FileStream(pathPANSIGN, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);
            foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)
            {
                for (int i = 1; i <= contextPanel.SIGN_LINE_LEN_BEFORE_HAB; i++)
                    writeBinary.Write(fushab09Indiv.AVANT[i]);

                for (int j = 1; j <= NBSTA; j++)
                    for (int i = 1; i <= 9; i++)
                        writeBinary.Write(fushab09Indiv.KHAB[i, j]);

                for (int j = 1; j <= NBSTA; j++)
                    for (int i = 1; i <= 9; i++)
                        writeBinary.Write(fushab09Indiv.KHSA[i, j]);

                for (int j = 1; j <= NBSTA; j++)
                    for (int i = 1; i <= 9; i++)
                        writeBinary.Write(fushab09Indiv.KHDI[i, j]);

                for (int i = 1; i <= contextPanel.SIGN_LINE_LEN_AFTER_HAB; i++)
                    writeBinary.Write(fushab09Indiv.APRES[i]);

                for (int i = 1; i <= 2; i++)
                    writeBinary.Write(fushab09Indiv.CHARIOT[i]);
            }

            writeBinary.Close();
            writeStream.Close();

            return fushab09Indivs;
        }

        private int[][][][] chab1qhp(int NBSTA, List<Fushab09Indiv> fushab09Indivs, int[] ISTA, string pathHab)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES HABITUDES 1 / 4h PAR INDIVIDU

            //  Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)
            int NBIND = fushab09Indivs.Count;
            int[][][][] NINI = new int[NBSTA + 1][][][]; //   NBSTA + 1 ,  3 + 1 , 96 + 1, NBIND + 1];
            for (int i=1; i<= NBSTA; i++)
            {
                NINI[i] = new int[3 + 1][][];
                for (int j = 1; j <= 3; j++)
                {
                    NINI[i][j] = new int[96 + 1][];
                    for (int k = 1; k <= 96; k++)
                    {
                        NINI[i][j][k] = new int[NBIND + 1];
                    }

                }

            }

            // Ecriture habitude
            FileStream writeStream = new FileStream(pathHab, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);

            // id  time
            //   1     05h00 - 06h00 = 4
            //   2     06h00 - 09h00 = 12
            //   3     09h00 - 12h00 = 12
            //   4     12h00 - 14h00 = 8
            //   5     14h00 - 16h00 = 8
            //   6     16h00 - 18h00 = 8
            //   7     18h00 - 20h00 = 8
            //   8     20h00 - 24h00 = 16
            //   9     24h00 - 05h00 = 20

            for (int IPO = 1; IPO <= NBSTA; IPO++)
            {
                int IP = IPO;
                Console.WriteLine($" Traitement station {IPO}");
                int IG = 0;

                // BOUCLE INDIVIDUS
                foreach (Fushab09Indiv indiv in fushab09Indivs)
                {
                    IG = IG + 1;
                    for (int IU = 1; IU <= 3; IU++)
                    {
                        for (int IQ = 1; IQ <= 96; IQ++)
                        {
                            int IH = 0;
                            if (IU == 1) IH = indiv.KHAB[ITH[IQ], IP] - 48;
                            if (IU == 2) IH = indiv.KHSA[ITH[IQ], IP] - 48;
                            if (IU == 3) IH = indiv.KHDI[ITH[IQ], IP] - 48;
                            if (IH < 1) IH = 5;
                            NINI[IPO][IU][IQ][IG] = 5 - IH;
                            if (ISTA[IPO] == 1 && IH != 5) NINI[IPO][IU][IQ][IG] = 1;
                        }
                    }
                }
                for (int IU = 1; IU <= 3; IU++)
                {
                    for (int IQ = 1; IQ <= 96; IQ++)
                    {
                        for (IG = 1; IG <= NBIND; IG++)
                        {
                            writeBinary.Write(Convert.ToByte(NINI[IPO][IU][IQ][IG]));
                        }
                    }
                }
            }

            writeBinary.Close();
            writeStream.Close();

            return NINI;
        }

        private int[,] crenonin(ContextPanel contextPanel, int NBJOUR, int NBSTA, List<Fushab09Indiv> fushab09Indivs, VsorPoid[][] JN, List<Tuple<int, int>> stationApres, string pathNinities)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CREATION DU FICHIER DES NON INITIALISES AUX STATIONS

            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)
            //PARAMETER(NBSTA =#NB_STA_HAB_NOTO#)
            //PARAMETER(NBJOUR = 23)
            //PARAMETER(NBIND =#NB_INDIV#)
            //PARAMETER(LENENR =#JFC_FILE_LEN_ENR#)

            int COMPT;
            int NBIND = fushab09Indivs.Count;

            // Attention AVANT[#SIGN_LINE_LEN_BEFORE_HAB#) et le buffer de lecture pour se caler au début des données de HAB
            // Attention APRES[#SIGN_LINE_LEN_AFTER_HAB#) et le buffer de lecture pour se caler à la fin de la ligne des données
            int[,] KHI2 = new int[NBIND + 1, NBSTA + 1];

            // INITIALISATIONS
            int IG = 0;

            // BOUCLE INDIVIDUS
            foreach (Fushab09Indiv indiv in fushab09Indivs)
            {
                if (IG % 100 == 0) Console.WriteLine($"IG {IG}");
                IG = IG + 1;
                // BOUCLE STATIONS
                for (int NOP = 1; NOP <= NBSTA; NOP++)
                {
                    int IP = NOP;
                    int IPO = 6 * (IP - 1) + 1;
                    int NOSH = NOP;
                    KHI2[IG, NOP] = 0;

                    // NON INITIALISE ?
                    // Pour les stations dont on dispose des habitudes d'écoutes, 
                    // on regarde si l'individu à déclaré une habitude
                    // d'écoute sur au moins une des 9 tranches horaires de la journée, sur la semaine, sur le
                    // samedi et enfin, sur le dimanche. Dès qu'on détecte une habitude, on sors de la boucle et
                    // la valeur de KHI2 pour l'individu sur la station restera à 0 (c'est à dire que l'individu
                    // est initialisé). Sinon, on regarde si l'individu a déclaré avoir écouté au moins un 1/4 d'heure
                    // de la station et si encore il n'a rien écouté alors l'individu est dit non initialisé et KHI2(IG, NOP) = 1

                    bool sortie = false;
                    if (
                        (contextPanel.Enquete != Enquete.PanelIleDeFrance && (NOP != 20) && (NOP != 21) && (NOP != 22) && (NOP != 23) && (NOP != 24) && (NOP != 25) && (NOP != 26) && (NOP != 27))
                        ||
                        (contextPanel.Enquete == Enquete.PanelIleDeFrance && (NOP != 1) && (NOP != 19) && (NOP != 20) && (NOP != 21) && (NOP != 24) && (NOP != 26) && (NOP != 27) && (NOP != 28) && (NOP != 29) && (NOP != 30) && (NOP != 31) && (NOP != 32))
                        )
                    {
                        for (int ITR = 1; ITR <= 9; ITR++)
                        {
                            if (((indiv.KHAB[ITR, NOSH] - 48) != 5 && (indiv.KHAB[ITR, NOSH] - 48) != 0) ||
                                ((indiv.KHSA[ITR, NOSH] - 48) != 5 && (indiv.KHSA[ITR, NOSH] - 48) != 0) ||
                                ((indiv.KHDI[ITR, NOSH] - 48) != 5 && (indiv.KHDI[ITR, NOSH] - 48) != 0))
                            {
                                sortie = true;
                                break;
                            }
                        }

                    }
                    if (sortie) continue;
                    // id  idx sta
                    // 16  101    Les Indés Radios 1
                    // 47  201    NRJ Global Proximité Premium 20
                    // 50  303    Nova and Friends 21
                    // 53  305    Lagardère Publicité News IDF 22
                    // 55  306    Lip !24
                    // 56  307    Paris – IDF + 25
                    // 60  310    Les Indés Capitale 27
                    // 61  311    TF1 Pub Radios 28

                    foreach (Tuple<int, int> t in stationApres)
                    {
                        if (NOP == t.Item1 && (indiv.APRES[t.Item2] - 48) == 1)
                        {
                            sortie = true;
                            break;
                        }
                    }

                    if (!sortie)
                    {
                        for (int I = 1; I <= NBJOUR; I++)
                        {
                            for (int IQ = 1; IQ <= 96; IQ++)
                            {
                                ushort[] bits = new ushort[7];
                                for (int b = 1; b <= 6; b++)
                                    bits[b] = JN[I][IG].VSor[b, NOP];

                                if (L1BITFCT(bits, IQ))
                                {
                                    sortie = true;
                                    break;
                                }
                            }
                            if (sortie) break;
                        }
                        if (!sortie) KHI2[IG, NOP] = 1;
                    }
                }
            }

            // Ecriture ninities
            FileStream writeStream = new FileStream(pathNinities, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);
            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
                // BOUCLE INDIVIDUS
                IG = 0;
                foreach (Fushab09Indiv indiv in fushab09Indivs)
                {
                    IG = IG + 1;
                    writeBinary.Write(Convert.ToByte(KHI2[IG, NOP]));
                }
            }
            writeBinary.Close();
            writeStream.Close();

            for (int I = 1; I <= NBSTA; I++)
            {
                COMPT = 0;
                for (int J = 1; J <= NBIND; J++)
                {
                    if (KHI2[J, I] == 1) COMPT = COMPT + 1;
                }
                Console.WriteLine($"Compt = {COMPT}");
            }

            return KHI2;
        }

        private void ecrsegpa(ContextPanel contextPanel, string pathSig, string pathSegs, int NBSTA, int NBIND, int SIGN_LINE_LEN_FULL, string pathSortie5,
            out int[,] SEGM, out int IPOP)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // ECRITURE DES SEGMENTS ET DU POIDS

            // PARAMETER(NBIND =#NB_INDIV#)

            // Longueur d'une ligne: 599 + 2 car de fin
            //int[] KHI2 = new int[SIGN_LINE_LEN_FULL + 1];
            SEGM = new int[NBIND + 1, contextPanel.TRegTest + 1];
            int ICSP, IREG, ISEX, IAGE, ISEG;
            int[] C = new int[17];
            IPOP = 0;

            // OUVERTURE FICHIER
            IEnumerable<string> KHI2 = File.ReadLines(pathSig);

            // BOUCLE INDIVIDUS
            int IG = 0;
            //while (fs.Position != fs.Length)
            foreach (string strIndiv in KHI2)
            {
                ISEG = 0;
                // CALCUL DU POIDS(Colonnes 9 à 13 inclues)
                int IPERS = int.Parse(strIndiv.Substring(contextPanel.COL_PIAB, 5));

                if (IPERS > 0)
                {
                    IG = IG + 1;
                    SEGM[IG, 1] = IPERS;
                    IPERS *= 10;
                    IPOP += IPERS;

                    // DETERMINATION DU SEGMENT NIVEAU 1
                    //------------------ -
                    // CSP de l'interviewé
                    //------------------ -
                    // La CSP de l'individu est codé sur une colonne (colonne #COL,CSCI#)
                    // ICSP = 1 correspond aux CSP +
                    // ICSP = 2 correspond aux CSP -
                    // ICSP = 3 correspond aux inactifs
                    ICSP = 1;
                    if ((strIndiv[contextPanel.COL_CSCI] == '1') || (int.Parse(strIndiv[contextPanel.COL_CSCI].ToString()) > 4)) ICSP = 2;
                    if (int.Parse(strIndiv[contextPanel.COL_CSCI].ToString()) > 6) ICSP = 3;

                    //--------------------
                    // Sexe de l'interviewé
                    //--------------------
                    // Le sexe de l'interviewé est codé sur une colonne (colonne #COL,SEXE#)
                    // ISEX = 1 correspond aux Hommes
                    // ISEX = 2 correspond aux Femmes
                    ISEX = int.Parse(strIndiv[contextPanel.COL_SEX].ToString());

                    //------------------ -
                    // Age de l'interviewé
                    //------------------ -
                    // L'Age de l'interviewé est codé sur deux colonnes(colonne #COL,AG11,0# et #COL,AG11,1#)
                    // IAGE = 1 correspond aux 13 - 34 ans
                    // IAGE = 2 correspond aux 35 - 59 ans
                    // IAGE = 3 correspond aux 60 ans et +
                    int AGE = int.Parse(strIndiv.Substring(contextPanel.COL_AGE11, 2));
                    IAGE = 1;
                    if (contextPanel.Age35a59.Contains(AGE)) IAGE = 2;
                    if (contextPanel.Age60Plus.Contains(AGE)) IAGE = 3;

                    //--------------------------
                    // Région UDA de l'interviewé
                    //--------------------------
                    // La région UDA de l'interviewé est codé sur une colonne (colonne #COL,RUDA#)
                    // IREG = 1 correspond aux 6 UDA Nord
                    // IREG = 2 correspond aux 3 UDA Sud

                    IREG = 1;
                    if (int.Parse(strIndiv[contextPanel.COL_RUDA].ToString()) > 6) IREG = 2;

                    //-------------------------------------------------------------------------- -
                    // On fonction des variables précédemment calculées, on détermine les segments
                    //-------------------------------------------------------------------------- -
                    if (contextPanel.Enquete == Enquete.PanelIleDeFrance)
                    {
                        if ((ISEX == 1) && (IAGE == 1)) ISEG = 1;
                        // Segment 2 = Femmes de 13 à 34 ans
                        if ((ISEX == 2) && (IAGE == 1)) ISEG = 2;
                        // Segment 3 = Hommes de 35 à 59 ans
                        if ((ISEX == 1) && (IAGE == 2)) ISEG = 3;
                        // Segment 4 = Femmes de 35 à 59 ans
                        if ((ISEX == 2) && (IAGE == 2)) ISEG = 4;
                        // Segment 5 = 60 ans et +
                        if (IAGE == 3) ISEG = 5;
                    }
                    else if (contextPanel.Enquete != Enquete.PanelIleDeFrance)
                    {
                        // Segment 5 = 60 ans et +
                        if (IAGE != 3)
                        {
                            if (ICSP == 1)
                            {
                                ISEG = 3;
                                if (IREG == 1 && ISEX == 1) ISEG = 1;
                                if (IREG == 1 && ISEX == 2) ISEG = 2;
                            }
                            else
                            {
                                if (ICSP != 3)
                                {
                                    if (IAGE != 2)
                                    {
                                        ISEG = 4;
                                        if (ISEX == 2) ISEG = 5;
                                    }
                                    else
                                    {
                                        ISEG = 11;
                                        if (IREG == 1 && ISEX == 1) ISEG = 9;
                                        if (IREG == 1 && ISEX == 2) ISEG = 10;
                                    }
                                }
                                else
                                {
                                    if (IAGE != 2)
                                    {
                                        ISEG = 8;
                                        if (IREG == 1 && ISEX == 1) ISEG = 6;
                                        if (IREG == 1 && ISEX == 2) ISEG = 7;
                                    }
                                    else
                                    {
                                        ISEG = 12;
                                    }
                                }
                            }
                        }
                        else
                        {
                            ISEG = 13;
                            if (IREG == 2) ISEG = 14;
                            if (ISEX == 2) ISEG = ISEG + 2;
                        }
                    }
                }
                SEGM[IG, 2] = ISEG;
                SEGM[IG, 3] = contextPanel.SEG1[ISEG];
                SEGM[IG, 4] = contextPanel.SEG2[contextPanel.SEG1[ISEG]];
                if (contextPanel.Enquete != Enquete.PanelIleDeFrance) SEGM[IG, 5] = contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[ISEG]]];

                C[ISEG]++;
            }

            FileStream writeStream = new FileStream(pathSegs, FileMode.Create);
            BinaryWriter writeBinay = new BinaryWriter(writeStream);
            for (int J = 1; J <= contextPanel.TRegTest; J++)
            {
                for (int I = 1; I <= NBIND; I++)
                {
                    writeBinay.Write(Convert.ToInt16(SEGM[I, J]));
                }
            }
            writeBinay.Close();
            writeStream.Close();

            for (int i = 1; i <= 16; i++)
            {
                Console.WriteLine(C[i]);
            }

            if (File.Exists(pathSortie5)) File.Delete(pathSortie5);
            File.AppendAllText(pathSortie5, $"   NBGUS: {IG}   POPULATION:{IPOP}");
        }

        private short[,,,] calcregr(ContextPanel contextPanel, int NBIND, int NBSTA, int[,] NINI, int[,] POIDSEGM, VsorPoid[][] JN, string pathCellule)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CONTROLE DES MINIMA D'ECOUTES PAR CELLULES DE BASE          
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;
            int NBCEL = (contextPanel.Enquete == Enquete.PanelIleDeFrance ? 5 : 23);
            int[,] KHI2 = new int[NBIND + 1, 6 + 1];
            int[] IAUD = new int[3 + 1];
            short[,,,] MINIS = new short[3 + 1, NBSTA + 1, 96 + 1, NBCEL + 1];

            // BOUCLE INDIVIDUS
            for (int IG = 1; IG <= NBIND; IG++)
            {
                int IC = POIDSEGM[IG, 2];

                // BOUCLE STATIONS
                for (int NOP = 1; NOP <= NBSTA; NOP++)
                {
                    if (NINI[IG, NOP] == 0)
                    {
                        int IP = NOP;
                        int IPO = 6 * (IP - 1) + 1;
                        //  BOUCLE TRANCHE HORAIRE
                        for (int IQ = 1; IQ <= 96; IQ++)
                        {
                            IAUD[1] = 0;
                            IAUD[2] = 0;
                            IAUD[3] = 0;
                            for (int I = 1; I <= NBJOUR; I++)
                            {
                                int IL = I;
                                int IU = 1;
                                if (I == 1 || I == 8 || I == 15 || I == 22) IU = 2;
                                if (I == 2 || I == 9 || I == 16 || I == 23) IU = 3;

                                ushort[] bits = new ushort[7];
                                for (int b = 1; b <= 6; b++)
                                    bits[b] = JN[I][IG].VSor[b, NOP];

                                if (L1BITFCT(bits, IQ)) IAUD[IU] = 1;
                            }
                            if (IAUD[1] == 1) MINIS[1, NOP, IQ, IC]++;
                            if (IAUD[2] == 1) MINIS[2, NOP, IQ, IC]++;
                            if (IAUD[3] == 1) MINIS[3, NOP, IQ, IC]++;
                        }
                    }
                }
            }

            // Ecriture CELLULES
            FileStream writeStream = new FileStream(pathCellule, FileMode.Create);
            BinaryWriter writeBinay = new BinaryWriter(writeStream);
            // BOUCLE STATIONS
            for (int IC = 1; IC <= (contextPanel.Enquete == Enquete.PanelIleDeFrance ? 5 : 23); IC++)
            {
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    for (int STA = 1; STA <= NBSTA; STA++)
                    {
                        for (int I = 1; I <= 3; I++)
                        {
                            writeBinay.Write(Convert.ToInt16(MINIS[I, STA, IQ, IC]));
                        }
                    }
                }
            }
            writeBinay.Close();
            writeStream.Close();

            Console.WriteLine("7h15 :");
            for (int J = 1; J <= NBCEL; J++)
                Console.WriteLine(MINIS[2, 2, 30, J]);
            Console.WriteLine("7h30 :");
            for (int J = 1; J <= NBCEL; J++)
                Console.WriteLine(MINIS[2, 2, 31, J]);
            Console.WriteLine("7h45 :");
            for (int J = 1; J <= NBCEL; J++)
                Console.WriteLine(MINIS[2, 2, 32, J]);

            return MINIS;
        }

        private byte[,,,] calcnivo(ContextPanel contextPanel, int NBIND, int NBSTA, short[,,,] MINIS, string pathNivo)
        {
            // C PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // REGROUPEMENTS DES CELLULES DE BASE
            // 
            int NBCEL = contextPanel.NbSeg;

            // Seuil de regoupement des cellules (nombre d'indivs)
            int NIVREG = 9;

            // Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)
            byte[,,,] REGRS = new byte[3 + 1, NBSTA + 1, 96 + 1, NBCEL + 1];
            byte[] TREG = new byte[NBCEL + 1];

            // BOUCLE STATIONS
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                // BOUCLE UNIVERS
                for (int IU = 1; IU <= 3; IU++)
                {
                    // BOUCLE TRANCHE HORAIRE
                    for (int IQ = 1; IQ <= 96; IQ++)
                    {
                        int N = 0;
                        for (int IC = 1; IC <= NBCEL; IC++)
                        {
                            TREG[IC] = 1;
                            if (MINIS[IU, IP, IQ, IC] > 0) N = 1;
                            if (MINIS[IU, IP, IQ, IC] >= NIVREG) TREG[IC] = 0;
                        }
                        if (N == 0) // **IP IU IQ
                        {
                            for (int IC = 1; IC <= NBCEL; IC++)
                            {
                                TREG[IC] = Convert.ToByte(contextPanel.TRegTest);
                            }// **GOTO100
                        }
                        else
                        {// **ELSE
                            // REGROUPEMENT NIVEAU N1 >> NIVEAU N2
                            bool sortie = false;
                            for (byte IN = 1; IN <= (contextPanel.Enquete == Enquete.PanelIleDeFrance ? 2 : 3); IN++)
                            {
                                for (int IC = 1; IC <= NBCEL; IC++)
                                {
                                    if (TREG[IC] == IN)
                                    {
                                        for (int I = 1; I <= NBCEL; I++)
                                        {
                                            if (contextPanel.NIVO[IN, I] == contextPanel.NIVO[IN, IC]) TREG[I] = TREG[IC];
                                        }
                                    }
                                }
                                N = 0; // **TREG
                                for (int IC = 1; IC <= NBCEL; IC++)
                                {
                                    if (TREG[IC] == IN)
                                    {
                                        int IM = 0;

                                        for (int I = 1; I <= NBCEL; I++)
                                        {
                                            if (contextPanel.NIVO[IN, I] == contextPanel.NIVO[IN, IC]) IM = IM + MINIS[IU, IP, IQ, I];
                                        }
                                        // ** IM
                                        if (IM < NIVREG)
                                        {
                                            N = 1;
                                            for (int I = 1; I <= NBCEL; I++)
                                            {
                                                if (contextPanel.NIVO[IN, I] == contextPanel.NIVO[IN, IC]) TREG[I] = Convert.ToByte(IN + 1);
                                            } // ** TREG5
                                        }
                                    }
                                }
                                // **TREG2
                                if (N == 0)
                                {
                                    sortie = true;
                                    break;
                                }
                            }

                            if (!sortie)
                            {
                                for (int IC = 1; IC <= NBCEL; IC++)
                                {
                                    if (TREG[IC] == contextPanel.TRegTest - 1)
                                    {
                                        for (int I = 1; I <= NBCEL; I++)
                                        {
                                            TREG[I] = Convert.ToByte(contextPanel.TRegTest - 1);
                                        }
                                    }
                                }// ** TREG3
                            }
                        }
                        for (int IC = 1; IC <= NBCEL; IC++)
                        {
                            REGRS[IU, IP, IQ, IC] = TREG[IC];
                        }
                    }
                }
            }

            if (File.Exists(pathNivo)) File.Delete(pathNivo);
            FileStream writeStream = new FileStream(pathNivo, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);
            for (int IC = 1; IC <= NBCEL; IC++)
            {
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    for (int IP = 1; IP <= NBSTA; IP++)
                    {
                        for (int IU = 1; IU <= 3; IU++)
                        {
                            writeBinary.Write(Convert.ToSByte(REGRS[IU, IP, IQ, IC]));
                        }
                    }
                }
            }

            writeBinary.Close();
            writeStream.Close();

            for (int I = 1; I <= NBCEL; I++)
                Console.WriteLine(MINIS[2, 2, 11, I]);

            return REGRS;
        }

        // VsorPoid[][] JN [Jour 1..23][Individus 1..N] = {VOSR[,]?, Poid[]}
        // retour [Station, INdiv, Qh, 1..3]
        private byte[][][][] caud1qhp(int NBIND, int NBSTA, VsorPoid[][] JN, int[,] POIDSEGM, string pathAudQhInd)
        {
            // C PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES AUDIENCES PAR INDIVIDU/ QUARTS d'HEURE/STATIONS
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;

            byte[][][][] NIN2 = new byte[NBSTA + 1][][][]; //    ,3 + 1 , 96 + 1, NBIND + 1];
            for (int i = 1; i <= NBSTA; i++)
            {
                NIN2[i] = new byte[3 + 1][][];
                for (int j = 1; j <= 3; j++)
                {
                    NIN2[i][j] = new byte[96 + 1][];
                    for (int k = 1; k <= 96; k++)
                    {
                        NIN2[i][j][k] = new byte[NBIND + 1];
                    }
                }
            }

            // OPEN(15, FILE = '#OUTPUT#audqhind',
            //-FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')
            // Ecriture AuqQhInd
            FileStream writeStream = new FileStream(pathAudQhInd, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);

            for (int STA = 1; STA <= NBSTA; STA++)
            {
                // BOUCLE INDIVIDUS
                for (int IG = 1; IG <= NBIND; IG++)
                {

                    for (int IJ = 1; IJ <= NBJOUR; IJ++)
                    {
                        int IU = 1;
                        if (IJ == 1 || IJ == 8 || IJ == 15 || IJ == 22) IU = 2;
                        if (IJ == 2 || IJ == 9 || IJ == 16 || IJ == 23) IU = 3;

                        for (int IQ = 1; IQ <= 96; IQ++)
                        {
                            ushort[] bits = new ushort[7];
                            for (int b = 1; b <= 6; b++)
                                bits[b] = JN[IJ][IG].VSor[b, STA];

                            if (L1BITFCT(bits, IQ))
                            {
                                NIN2[STA][IU][IQ][IG] = Convert.ToByte(NIN2[STA][IU][IQ][IG] + JN[IJ][IG].Poid[IQ]);
                            }
                        }
                    }
                }

                // CALCUL DE LA NOTE INDIVIDUELLE
                for (int I = 1; I <= 3; I++)
                {
                    for (int IQ = 1; IQ <= 96; IQ++)
                    {
                        for (int IG = 1; IG <= NBIND; IG++)
                        {
                            writeBinary.Write(Convert.ToByte(NIN2[STA][I][IQ][IG]));
                        }
                    }
                }
            }

            writeBinary.Close();
            writeStream.Close();

            return NIN2;
        }

        private float[] caudtotp(ContextPanel contextPanel, int NBIND, int NBSTA,
            VsorPoid[][] JN, int[,] POIDS, List<Fushab09Indiv> fushab09Indivs, string pathNoteIndiv)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES AUDIENCES TOTALES PAR INDIVIDUS / STATION
            // POUR DISTANCES ENTRE INDIVIDUS
            // Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)

            int NBJOUR = 23;

            // Dans les fichier .jfc, chaque enregistrement(individu) est de 180 short = 6 short(96 qh) * 30 stations

            float[] COUV = new float[NBIND + 1];
            float[] NOTE = new float[NBIND + 1];
            //int KHI3(LENENR, NBJOUR),POIDS(NBIND),AGE,INSEE,CELLULE,ETAPE,

            // Attention fushab09Indiv.AVANT[34) et le buffer de lecture pour se caler au début des données de HAB
            // Attention APRES[64) et le buffer de lecture pour se caler à la fin de la ligne des données
            int[,] NINI = new int[NBIND + 1, 20 + 1];


            // BOUCLE INDIVIDUS
            int IG = 0;
            foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)
            {
                IG++;

                for (int IJ = 1; IJ <= NBJOUR; IJ++)
                {
                    for (int IPO = 1; IPO <= NBSTA; IPO++)
                    {
                        for (int IQ = 1; IQ <= 96; IQ++)
                        {
                            ushort[] bits = new ushort[7];
                            for (int b = 1; b <= 6; b++)
                                bits[b] = JN[IJ][IG].VSor[b, IPO];
                            if (L1BITFCT(bits, IQ))
                            {
                                COUV[IG] += JN[IJ][IG].Poid[IQ];
                            }
                        }
                    }
                }

                //  **CRITERES**
                // SEXE
                NINI[IG, 1] = fushab09Indiv.AVANT[contextPanel.COL_TAB_SEX] - 48;
                // AGE
                int AGE = 10 * (fushab09Indiv.AVANT[contextPanel.COL_TAB_AGE11] - 48) + (fushab09Indiv.AVANT[contextPanel.COL_TAB_AGE11 + 1] - 48);
                NINI[IG, 2] = 1;
                //if (AGE > 2) NINI[IG, 2] = 2; // 25-34 ans
                //if (AGE > 4) NINI[IG, 2] = 3; // >= 35  <=49
                //if (AGE > 7) NINI[IG, 2] = 4; // >= 50  <= 64
                //if (AGE > 10) NINI[IG, 2] = 5; // >65

                NINI[IG, 2] = 1; // < 25
                if (contextPanel.Age25a34.Contains(AGE)) NINI[IG, 2] = 2;  // >= 25  <= 34
                if (contextPanel.Age35a49.Contains(AGE)) NINI[IG, 2] = 3;  // >= 35  <=49
                if (contextPanel.Age50a64.Contains(AGE)) NINI[IG, 2] = 4;  // >= 50  <= 64
                if (contextPanel.Age65Plus.Contains(AGE)) NINI[IG, 2] = 5; // >65
                // AGE
                NINI[IG, 3] = 1;
                //if (AGE > 7) NINI[IG, 3] = 2;
                if (contextPanel.Age50a64.Contains(AGE) || contextPanel.Age65Plus.Contains(AGE)) NINI[IG, 3] = 2;  // >= 50  <= 64
                // AGE
                NINI[IG, 4] = 1;
                //if (AGE > 1) NINI[IG, 4] = 2; // >= 20
                if (contextPanel.Age20a24.Contains(AGE) || contextPanel.Age25a34.Contains(AGE) || contextPanel.Age35a49.Contains(AGE) || 
                    contextPanel.Age50a64.Contains(AGE) || contextPanel.Age65Plus.Contains(AGE))
                    NINI[IG, 4] = 2;
                // Profession Individu
                NINI[IG, 5] = fushab09Indiv.AVANT[contextPanel.COL_TAB_CSCI] - 48;
                // Région UDA
                NINI[IG, 6] = fushab09Indiv.AVANT[contextPanel.COL_TAB_RUDA] - 48;
                // Région UDA
                NINI[IG, 7] = 1;
                if ((fushab09Indiv.AVANT[contextPanel.COL_TAB_RUDA] - 48) > 1) NINI[IG, 7] = 2;
                if ((fushab09Indiv.AVANT[contextPanel.COL_TAB_RUDA] - 48) > 6) NINI[IG, 7] = 3;
                // Habitat
                NINI[IG, 8] = 1;
                if ((fushab09Indiv.AVANT[contextPanel.COL_TAB_HAB] - 48) > 1) NINI[IG, 8] = 2;
                if ((fushab09Indiv.AVANT[contextPanel.COL_TAB_HAB] - 48) > 2) NINI[IG, 8] = 3;
                if ((fushab09Indiv.AVANT[contextPanel.COL_TAB_HAB] - 48) > 4) NINI[IG, 8] = 4;
                if ((fushab09Indiv.AVANT[contextPanel.COL_TAB_HAB] - 48) > 6) NINI[IG, 8] = 5;
                // Ménagère
                NINI[IG, 9] = (fushab09Indiv.AVANT[contextPanel.COL_TAB_MENA] - 48);
                // Responsable des achats
                NINI[IG, 10] = (fushab09Indiv.AVANT[contextPanel.COL_TAB_RDA] - 48);
                // Profession du chef de famille
                NINI[IG, 11] = (fushab09Indiv.AVANT[contextPanel.COL_TAB_CSCC] - 48);
                // Région NIELSEN
                NINI[IG, 12] = (fushab09Indiv.AVANT[contextPanel.COL_TAB_NIEL] - 48);
                // Région INSEE
                int INSEE = 10 * (fushab09Indiv.AVANT[contextPanel.COL_TAB_PG22] - 48) + (fushab09Indiv.AVANT[contextPanel.COL_TAB_PG22 + 1] - 48);
                NINI[IG, 13] = INSEE;
                // Nb d'enfants de moins de 6 ans
                NINI[IG, 14] = fushab09Indiv.AVANT[contextPanel.COL_TAB_ENF1] - 48;
                // Nb d'enfants de 6 à 8 ans
                NINI[IG, 15] = fushab09Indiv.AVANT[contextPanel.COL_TAB_ENF2] - 48;
                // Nb d'enfant de 9 à 10 ans
                NINI[IG, 16] = fushab09Indiv.AVANT[contextPanel.COL_TAB_ENF3] - 48;
                // Nb d'enfant de 11 à 14 ans
                NINI[IG, 17] = fushab09Indiv.AVANT[contextPanel.COL_TAB_ENF4] - 48;
                // Nb de personnes vivant dans le foyer
                NINI[IG, 18] = fushab09Indiv.AVANT[contextPanel.COL_TAB_NPER] - 48;
                // Etape de la vie
                int ETAPE = 10 * (fushab09Indiv.APRES[contextPanel.COL_TAB_APRES] - 48) + fushab09Indiv.APRES[contextPanel.COL_TAB_APRES + 1] - 48;
                NINI[IG, 19] = ETAPE;
                //Console.WriteLine(ETAPE);
                // Cellule
                if (contextPanel.Enquete == Enquete.PanelIleDeFrance)
                    NINI[IG, 20] = fushab09Indiv.AVANT[contextPanel.COL_TAB_ENFA] - 48;
                else
                {
                    int CELLULE = 10 * (fushab09Indiv.AVANT[contextPanel.COL_TAB_CELL] - 48) + (fushab09Indiv.AVANT[contextPanel.COL_TAB_CELL + 1] - 48);
                    NINI[IG, 20] = CELLULE;
                }
            }

            // CALCUL DE LA NOTE INDIVIDUELLE
            float MINOT = 100000;
            float MANOT = 0;
            for (int I = 1; I <= NBIND; I++)
            {
                if (I % 100 == 0) Console.WriteLine($"IG : {I}");
                float NUM = 0f;
                float DEN = 0f;

                for (int J = 1; J <= NBIND; J++)
                {
                    int DIF = 0;
                    int NO = 25;
                    for (int IC = 1; IC <= 20; IC++)
                    {
                        if (IC == 9) NO = 1;
                        if (NINI[I, IC] != NINI[J, IC]) DIF += NO;
                    }
                    float D = -1f * ((DIF / 25f) * (DIF / 25f));
                    float A = POIDS[J, 1] * (float)Math.Exp(D);

                    DEN += A;
                    NUM += A * (COUV[J] / 12f);
                }

                NOTE[I] = NUM / DEN;
            }

            for (int I = 1; I <= NBIND; I++)
            {
                if (NOTE[I] < MINOT) MINOT = NOTE[I];

                if (NOTE[I] > MANOT) MANOT = NOTE[I];
            }

            FileStream writeStream = new FileStream(pathNoteIndiv, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);
            for (int I = 1; I <= NBIND; I++)
            {
                writeBinary.Write(NOTE[I]);
            }
            writeBinary.Close();
            writeStream.Close();
            Console.WriteLine($"MINOT {MINOT} MANOT {MANOT}");

            return NOTE;
        }

        private float[,,,] sav1qhpa(ContextPanel contextPanel, int NBIND, int NBSTA, byte[,,,] regrs, int[,] KHI2, List<Fushab09Indiv> fushab09Indivs, VsorPoid[][][] JNByWeek, VsorPoid[][] JN, string pathZuptause, string pathSortie)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES PARAMETRES STATION 2 QUART d'HEURE 31
            //(EUROPE 1 - 7H30 - 7H45)
            // LUNDI-VENDREDI
            // VERSION 2
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            double[,] C = new double[15 + 1, contextPanel.TRegTest + 1];
            double[] NCOM = new double[15 + 1] { 999999, 15, 105, 455, 1365, 3003, 5005, 6435, 6435, 5005, 3003, 1365, 455, 105, 15, 1 };
            float[,,,] RESUL = new float[NBSTA + 1, 96 + 1, 4 + 1, contextPanel.NbSeg + 1];
            double[] Z = new double[15 + 1];
            double[] YR = new double[15 + 1];

            int[,] COMPT = new int[16 + 1, contextPanel.TRegTest + 1];  // contextPanel.NbSeg + 1 ou 16 ???
            int[] ITPO = new int[contextPanel.TRegTest + 1];
            int[] ITTP = new int[contextPanel.TRegTest + 1];
            int[] IZAP = new int[contextPanel.TRegTest + 1];
            int[] IPPS = new int[contextPanel.TRegTest + 1];
            int[] ITJP = new int[contextPanel.TRegTest + 1];
            int[,] SEGM = new int[16 + 1, contextPanel.TRegTest + 1]; // contextPanel.NbSeg + 1 ou 16 ???
            int[] NB = new int[contextPanel.TRegTest + 1];
            int[] ITTL = new int[contextPanel.TRegTest + 1];
            int[] ITOU = new int[contextPanel.TRegTest + 1];
            int[] COMB = new int[15 + 1];
            int[] IZAB = new int[contextPanel.TRegTest + 1];
            int[] ITJR = new int[contextPanel.TRegTest + 1];
            int[] IM = new int[contextPanel.TRegTest + 1];
            int[,,] PATRON = new int[96 + 1, 15 + 1, NBIND + 1];
            int[,,] JATOU = new int[96 + 1, 4 + 1, NBIND + 1];
            int[] TREG = new int[contextPanel.NbSeg + 1];

            //  id  time
            //  1     05h00 - 06h00 = 4
            //  2     06h00 - 09h00 = 12
            //  3     09h00 - 12h00 = 12
            //  4     12h00 - 14h00 = 8
            //  5     14h00 - 16h00 = 8
            //  6     16h00 - 18h00 = 8
            //  7     18h00 - 20h00 = 8
            //  8     20h00 - 24h00 = 16
            //  9     24h00 - 05h00 = 20
            double ZR = 0f;
            double UR = 0f;
            double PR = 0f;
            double TAU = 0f;

            StringBuilder sbSortie = new StringBuilder();

            // OPEN(18, FILE = '#OUTPUT#ZUPTAUSE',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            FileStream writeStream = new FileStream(pathZuptause, FileMode.Create);
            BinaryWriter writeBinaryZuptause = new BinaryWriter(writeStream);


            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
                sbSortie.AppendLine("Station " + NOP.ToString());

                // INITIALISATIONS
                int IP = NOP;
                int IPO = 6 * (IP - 1) + 1;
                int NOSH = NOP;

                // BOUCLE INDIVIDUS
                // 30
                int IG = 0;
                foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)
                {
                    IG = IG + 1;

                    // BOUCLE 1 / 4h
                    for (int IQ = 1; IQ <= 96; IQ++)
                    {
                        int IH = fushab09Indiv.KHAB[ITH[IQ], NOSH] - 48;
                        if (IH == 0) IH = 5;
                        int NJOU = 0;
                        for (int IJ = 1; IJ <= 15; IJ++)
                        {
                            PATRON[IQ, IJ, IG] = 0;
                        }
                        for (int IJ = 1; IJ <= 4; IJ++)
                        {
                            JATOU[IQ, IJ, IG] = 0;
                        }
                        for (int I = 1; I <= contextPanel.NbSeg; I++)
                        {
                            TREG[I] = regrs[1, IP, IQ, I];
                        }

                        if (TREG[1] == contextPanel.TRegTest) continue;
                        int IJK = 1;
                        for (int I = 3; I <= 21; I++)
                        {
                            if (I != 8 && I != 9 && I != 15 && I != 16)
                            {
                                ushort[] bits = new ushort[7];
                                for (int b = 1; b <= 6; b++)
                                    bits[b] = JN[I][IG].VSor[b, NOP];

                                if (L1BITFCT(bits, IQ))
                                {
                                    PATRON[IQ, IJK, IG] = 1;
                                    NJOU = NJOU + 1;
                                }
                                IJK = IJK + 1;
                            }
                        }
                        if (NJOU == 0) JATOU[IQ, 1, IG] = 1;
                        if (NJOU == 0 && IH == 5) JATOU[IQ, 2, IG] = 1;
                        if (NJOU == 15) JATOU[IQ, 3, IG] = 1;
                        if (NJOU == 15 && IH == 1) JATOU[IQ, 4, IG] = 1;
                    } //  for (int IQ = 1; IQ <= 96; IQ++)
                } // foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)

                // 120 
                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    if (IQ==24)
                    {

                    }
                    sbSortie.AppendLine($"IQ = {IQ}");
                    Console.WriteLine($"--------STATION {NOP} --------1/4h {IQ} ");
                    for (int I = 1; I <= contextPanel.NbSeg; I++)
                    {
                        TREG[I] = regrs[1, IP, IQ, I];
                    }

                    if (TREG[1] != contextPanel.TRegTest)
                    {
                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            int N2 = contextPanel.SEG1[N1];
                            int N3 = contextPanel.SEG2[N2];
                            int N4 = (contextPanel.Enquete != Enquete.PanelIleDeFrance) ? contextPanel.SEG3[N3] : 0;
                            NB[1] = N1;
                            NB[2] = N2;
                            NB[3] = N3;
                            NB[4] = (contextPanel.Enquete != Enquete.PanelIleDeFrance) ? N4 : 1;
                            if (contextPanel.Enquete != Enquete.PanelIleDeFrance) NB[5] = 1;
                            int NIV = TREG[N1] + 1;
                            if (((N1 == 1) || (NIV == 1))
                                ||
                               (!((NIV == 2 && contextPanel.SEG1[N1] == contextPanel.SEG1[N1 - 1]) ||
                                  (NIV == 3 && contextPanel.SEG2[contextPanel.SEG1[N1]] == contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]) ||
                                  (contextPanel.Enquete != Enquete.PanelIleDeFrance && NIV == 4 && contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]] == contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]]) ||
                                  (NIV == contextPanel.TRegTest))))
                            {
                                for (int I = 1; I <= 16; I++)   // 16 or contextPanel.NbSeg ?????
                                {
                                    for (int J = 1; J <= contextPanel.TRegTest; J++)
                                    {
                                        SEGM[I, J] = 0;
                                        COMPT[I, J] = 0;
                                        if (I <= 15) C[I, J] = 0f;
                                    }
                                }

                                for (int J = 1; J <= contextPanel.TRegTest; J++)
                                {
                                    IM[J] = 0;
                                    IPPS[J] = 0;
                                    ITOU[J] = 0;
                                    ITPO[J] = 0;
                                    ITTL[J] = 0;
                                    ITTP[J] = 0;
                                    IZAB[J] = 0;
                                    IZAP[J] = 0;
                                    ITJR[J] = 0;
                                    ITJP[J] = 0;
                                }

                                // BOUCLE INDIVIDUS
                                for (IG = 1; IG <= NBIND; IG++)
                                {
                                    int IPERS = KHI2[IG, 1];
                                    bool KHI2_IG_NIV_EQ_NB_NIV = (NIV == contextPanel.TRegTest && NB[NIV] == 1) || (KHI2[IG, NIV + 1] == NB[NIV]);
                                    IPERS = IPERS * 10;
                                    if (KHI2_IG_NIV_EQ_NB_NIV)
                                    {
                                        IM[NIV] = IM[NIV] + 1;
                                        IPPS[NIV] = IPPS[NIV] + IPERS;
                                    }
                                    int IAUD = 0;
                                    int NJOU = 0;
                                    for (int I = 1; I <= 15; I++)
                                    {
                                        COMB[I] = PATRON[IQ, I, IG];
                                        if (COMB[I] == 1)
                                        {
                                            IAUD = 1;
                                            NJOU = NJOU + 1;
                                        }
                                    }
                                    if (IAUD == 0)
                                    {
                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            COMPT[1, NIV] = COMPT[1, NIV] + IPERS;
                                            SEGM[1, NIV] = SEGM[1, NIV] + 1;
                                            if (JATOU[IQ, 2, IG] == 1) IZAB[NIV] = IZAB[NIV] + 1;
                                            if (JATOU[IQ, 2, IG] == 1) IZAP[NIV] = IZAP[NIV] + IPERS;
                                        }
                                    }
                                    if (IAUD == 1)
                                    {
                                        int IJ = NJOU + 1;
                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            COMPT[IJ, NIV] = COMPT[IJ, NIV] + IPERS;
                                            SEGM[IJ, NIV] = SEGM[IJ, NIV] + 1;

                                            ITOU[NIV] = ITOU[NIV] + 1;
                                            ITPO[NIV] = ITPO[NIV] + IPERS;
                                        }
                                        if (NJOU == 15)
                                        {
                                            if (KHI2_IG_NIV_EQ_NB_NIV)
                                            {
                                                ITTL[NIV] = ITTL[NIV] + 1;
                                                ITTP[NIV] = ITTP[NIV] + IPERS;
                                                if (JATOU[IQ, 4, IG] == 1)
                                                {
                                                    ITJR[NIV] = ITJR[NIV] + 1;
                                                    ITJP[NIV] = ITJP[NIV] + IPERS;
                                                }
                                            }
                                        }

                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            #region CALCUL DES MOYENNES

                                            // PREMIER JOUR
                                            float TOT = 0f;
                                            for (int I01 = 1; I01 <= 15; I01++)
                                                if (COMB[I01] == 1) TOT += IPERS;
                                            C[1, NIV] += TOT;

                                            // 2eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 14; I01++)
                                                for (int I02 = I01 + 1; I02 <= 15; I02++)
                                                    if (COMB[I01] == 1 || COMB[I02] == 1) TOT += IPERS;
                                            C[2, NIV] += TOT;

                                            // 3eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 13; I01++)
                                                for (int I02 = I01 + 1; I02 <= 14; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 15; I03++)
                                                        if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1) TOT += IPERS;

                                            C[3, NIV] += TOT;

                                            // 4eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 12; I01++)
                                                for (int I02 = I01 + 1; I02 <= 13; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 14; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 15; I04++)
                                                            if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                COMB[I04] == 1) TOT += IPERS;
                                            C[4, NIV] += TOT;

                                            // 5eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 11; I01++)
                                                for (int I02 = I01 + 1; I02 <= 12; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 13; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 14; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 15; I05++)
                                                                if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                    COMB[I04] == 1 || COMB[I05] == 1) TOT += IPERS;
                                            C[5, NIV] += TOT;

                                            // 6eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 10; I01++)
                                                for (int I02 = I01 + 1; I02 <= 11; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 12; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 13; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 14; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 15; I06++)
                                                                    if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                            COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1) TOT += IPERS;

                                            C[6, NIV] += TOT;

                                            // 7eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 9; I01++)
                                                for (int I02 = I01 + 1; I02 <= 10; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 11; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 12; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 13; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 14; I06++)
                                                                    for (int I07 = I06 + 1; I07 <= 15; I07++)
                                                                        if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                                COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1 ||
                                                                                COMB[I07] == 1) TOT += IPERS;
                                            C[7, NIV] += TOT;

                                            // 8eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 8; I01++)
                                                for (int I02 = I01 + 1; I02 <= 9; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 10; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 11; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 12; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 13; I06++)
                                                                    for (int I07 = I06 + 1; I07 <= 14; I07++)
                                                                        for (int I08 = I07 + 1; I08 <= 15; I08++)
                                                                            if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                                    COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1 ||
                                                                                    COMB[I07] == 1 || COMB[I08] == 1) TOT += IPERS;

                                            C[8, NIV] += TOT;

                                            // 9eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 7; I01++)
                                                for (int I02 = I01 + 1; I02 <= 8; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 9; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 10; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 11; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 12; I06++)
                                                                    for (int I07 = I06 + 1; I07 <= 13; I07++)
                                                                        for (int I08 = I07 + 1; I08 <= 14; I08++)
                                                                            for (int I09 = I08 + 1; I09 <= 15; I09++)
                                                                                if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                                        COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1 ||
                                                                                        COMB[I07] == 1 || COMB[I08] == 1 || COMB[I09] == 1) TOT += IPERS;

                                            C[9, NIV] += TOT;

                                            // 10eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 6; I01++)
                                                for (int I02 = I01 + 1; I02 <= 7; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 8; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 9; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 10; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 11; I06++)
                                                                    for (int I07 = I06 + 1; I07 <= 12; I07++)
                                                                        for (int I08 = I07 + 1; I08 <= 13; I08++)
                                                                            for (int I09 = I08 + 1; I09 <= 14; I09++)
                                                                                for (int I10 = I09 + 1; I10 <= 15; I10++)
                                                                                    if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                                            COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1 ||
                                                                                            COMB[I07] == 1 || COMB[I08] == 1 || COMB[I09] == 1 ||
                                                                                            COMB[I10] == 1) TOT += IPERS;

                                            C[10, NIV] = C[10, NIV] + TOT;

                                            // 11eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 5; I01++)
                                                for (int I02 = I01 + 1; I02 <= 6; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 7; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 8; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 9; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 10; I06++)
                                                                    for (int I07 = I06 + 1; I07 <= 11; I07++)
                                                                        for (int I08 = I07 + 1; I08 <= 12; I08++)
                                                                            for (int I09 = I08 + 1; I09 <= 13; I09++)
                                                                                for (int I10 = I09 + 1; I10 <= 14; I10++)
                                                                                    for (int I11 = I10 + 1; I11 <= 15; I11++)
                                                                                        if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                                                COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1 ||
                                                                                                COMB[I07] == 1 || COMB[I08] == 1 || COMB[I09] == 1 ||
                                                                                                COMB[I10] == 1 || COMB[I11] == 1) TOT += IPERS;
                                            C[11, NIV] = C[11, NIV] + TOT;

                                            // 12eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 4; I01++)
                                                for (int I02 = I01 + 1; I02 <= 5; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 6; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 7; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 8; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 9; I06++)
                                                                    for (int I07 = I06 + 1; I07 <= 10; I07++)
                                                                        for (int I08 = I07 + 1; I08 <= 11; I08++)
                                                                            for (int I09 = I08 + 1; I09 <= 12; I09++)
                                                                                for (int I10 = I09 + 1; I10 <= 13; I10++)
                                                                                    for (int I11 = I10 + 1; I11 <= 14; I11++)
                                                                                        for (int I12 = I11 + 1; I12 <= 15; I12++)
                                                                                            if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                                                    COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1 ||
                                                                                                    COMB[I07] == 1 || COMB[I08] == 1 || COMB[I09] == 1 ||
                                                                                                    COMB[I10] == 1 || COMB[I11] == 1 || COMB[I12] == 1) TOT += IPERS;


                                            C[12, NIV] = C[12, NIV] + TOT;

                                            // 13eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 3; I01++)
                                                for (int I02 = I01 + 1; I02 <= 4; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 5; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 6; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 7; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 8; I06++)
                                                                    for (int I07 = I06 + 1; I07 <= 9; I07++)
                                                                        for (int I08 = I07 + 1; I08 <= 10; I08++)
                                                                            for (int I09 = I08 + 1; I09 <= 11; I09++)
                                                                                for (int I10 = I09 + 1; I10 <= 12; I10++)
                                                                                    for (int I11 = I10 + 1; I11 <= 13; I11++)
                                                                                        for (int I12 = I11 + 1; I12 <= 14; I12++)
                                                                                            for (int I13 = I12 + 1; I13 <= 15; I13++)
                                                                                                if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                                                        COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1 ||
                                                                                                        COMB[I07] == 1 || COMB[I08] == 1 || COMB[I09] == 1 ||
                                                                                                        COMB[I10] == 1 || COMB[I11] == 1 || COMB[I12] == 1 ||
                                                                                                        COMB[I13] == 1) TOT += IPERS;

                                            C[13, NIV] = C[13, NIV] + TOT;

                                            // 14eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 2; I01++)
                                                for (int I02 = I01 + 1; I02 <= 3; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 4; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 5; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 6; I05++)
                                                                for (int I06 = I05 + 1; I06 <= 7; I06++)
                                                                    for (int I07 = I06 + 1; I07 <= 8; I07++)
                                                                        for (int I08 = I07 + 1; I08 <= 9; I08++)
                                                                            for (int I09 = I08 + 1; I09 <= 10; I09++)
                                                                                for (int I10 = I09 + 1; I10 <= 11; I10++)
                                                                                    for (int I11 = I10 + 1; I11 <= 12; I11++)
                                                                                        for (int I12 = I11 + 1; I12 <= 13; I12++)
                                                                                            for (int I13 = I12 + 1; I13 <= 14; I13++)
                                                                                                for (int I14 = I13 + 1; I14 <= 15; I14++)
                                                                                                    if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                                                        COMB[I04] == 1 || COMB[I05] == 1 || COMB[I06] == 1 ||
                                                                                                        COMB[I07] == 1 || COMB[I08] == 1 || COMB[I09] == 1 ||
                                                                                                        COMB[I10] == 1 || COMB[I11] == 1 || COMB[I12] == 1 ||
                                                                                                        COMB[I13] == 1 || COMB[I14] == 1)
                                                                                                        TOT += IPERS;

                                            C[14, NIV] = C[14, NIV] + TOT;

                                            // 15eme JOUR
                                            TOT = 0f;
                                            if (COMB[1] == 1 || COMB[2] == 1 || COMB[3] == 1 ||
                                                COMB[4] == 1 || COMB[5] == 1 || COMB[6] == 1 ||
                                                COMB[7] == 1 || COMB[8] == 1 || COMB[9] == 1 ||
                                                COMB[10] == 1 || COMB[11] == 1 || COMB[12] == 1 ||
                                                COMB[13] == 1 || COMB[14] == 1 || COMB[15] == 1) TOT = IPERS;

                                            C[15, NIV] = C[15, NIV] + TOT;
                                            #endregion Calcul des mioyennes
                                        }
                                    } // FIN BOUCLE INDIVIDUS
                                } // FOR IG

                                // IMPRESSION DES RESULTATS
                                int IN = TREG[N1] + 1;
                                double POPS = 0f;
                                for (int I = 1; I <= NBIND; I++)
                                {
                                    if ((IN == contextPanel.TRegTest && NB[IN] == 1) || KHI2[I, IN + 1] == NB[IN]) POPS = POPS + KHI2[I, 1];
                                }

                                POPS = POPS * 10d;
                                double DIRAC0 = COMPT[1, IN] / POPS;
                                double DIRAC1 = ITTP[IN] / POPS;
                                double PC1 = IZAP[IN] / POPS;
                                int IPOP = 0;
                                for (int I = 2; I <= 16; I++)
                                {
                                    IPOP = IPOP + COMPT[I, IN] * (I - 1);
                                }

                                for (int I = 2; I <= 15; I++)
                                {
                                    SEGM[I + 1, IN] += SEGM[I, IN];
                                    COMPT[I + 1, IN] += COMPT[I, IN];
                                }

                                double GRP = IPOP * 100d;
                                GRP = GRP / POPS / 15d;
                                PR = (IPOP - ITTP[IN] * 15) / 14d;

                                // TODO :erreur fortran : seul le sav1dhpa hors IDF ne contient pas ce test ?
                                // if (contextPanel.Enquete == Enquete.PanelIleDeFrance && POPS == (COMPT[1, IN] + ITTP[IN]))
                                if (contextPanel.Enquete == Enquete.PanelIleDeFrance && POPS == (COMPT[1, IN] + ITTP[IN]))
                                {
                                    ZR = COMPT[1, IN] / POPS;
                                    UR = ITTP[IN] / POPS;
                                    PR = 1d;
                                    TAU = 0d;
                                }
                                else
                                {
                                    PR = PR / (POPS - COMPT[1, IN] - ITTP[IN]);
                                    for (int I = 1; I <= 15; I++)
                                    {
                                        Z[I] = 1d * C[I, IN] / NCOM[I] / POPS * 100d;
                                    }

                                    // APPEL DE LA FONCTION DIFFERENCE

                                    GRP = GRP / 100d;
                                    double ZA = DIRAC0;
                                    ZR = IZAP[IN] / POPS;
                                    double UA = ITJP[IN] / POPS;
                                    UR = DIRAC1;

                                    //WRITE(16, *) GRP, ZA, ZR, UA, UR
                                    sbSortie.AppendLine($"GRP {GRP}  ZA {ZA}  ZR {ZR}  UA {UA}  UR {UR} IZAP {IZAP[IN]} POPS {POPS} NIV {NIV} IN {IN}");

                                    float ND = 15;
                                    MINIMISE(GRP, Z, ref PR, ref UR, ref ZR, out TAU, ZA, UA, ND); // float GRP, float[] ZC, ref float PR, ref float UR, ref float ZR, out float TAU, float ZA, float UA, float NB)

                                    double GRP0 = 0d;
                                    double GRPC = 0d;
                                    for (int I = 1; I <= 15; I++)
                                    {
                                        GRPC = GRPC + YR[I];
                                        GRP0 = GRP0 + Z[I];
                                    }
                                }
                            } // if (((N1 == 1) || (NIV == 1) ) 
                            RESUL[NOP, IQ, 1, N1] = Convert.ToSingle(ZR);
                            RESUL[NOP, IQ, 2, N1] = Convert.ToSingle(UR);
                            RESUL[NOP, IQ, 3, N1] = Convert.ToSingle(PR);
                            RESUL[NOP, IQ, 4, N1] = Convert.ToSingle(TAU);

                            sbSortie.AppendLine($"CELLULE {N1} Z {ZR} U {UR} P {PR} TAU {TAU}");

                        } // for (int N1 = 1; N1 <= 16; N1++)
                    }
                    else
                    {
                        sbSortie.AppendLine(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            RESUL[NOP, IQ, 1, N1] = 1f;
                            RESUL[NOP, IQ, 2, N1] = 0f;
                            RESUL[NOP, IQ, 3, N1] = 0f;
                            RESUL[NOP, IQ, 4, N1] = 0f;
                        }
                    }// if (TREG[1] != 5)

                    // FIN DE TRAITEMENT CELLULE
                    for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                    {
                        for (int I = 1; I <= 4; I++)
                        {
                            writeBinaryZuptause.Write(RESUL[NOP, IQ, I, N1]);
                        }
                    }

                } // for (int IQ = 1; IQ <= 96; IQ++)

            } // for (int NOP = 1; NOP <= NBSTA; NOP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");

            if (File.Exists(pathSortie)) File.Delete(pathSortie);
            File.AppendAllText(pathSortie, sbSortie.ToString());

            //FORMAT('CELLULE ',I2,' Z=',F9.7,' U=',F9.7,' P=',F9.7,' TAU=',F9.7) == ??? 
            //? Console.WriteLine($"CELLULE {I2} Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7)

            writeBinaryZuptause.Close();
            writeStream.Close();

            return RESUL;
        }

        private float[,,,] sav1qhps(ContextPanel contextPanel, int NBIND, int NBSTA, byte[,,,] regrs, int[,] KHI2, List<Fushab09Indiv> fushab09Indivs, VsorPoid[][][] JNByWeek, VsorPoid[][] JN, string pathZuptausa, string pathSortie)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES PARAMETRES z,u,p,tau
            // SAMEDI
            // VERSION 2
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;

            double[,] C = new double[15 + 1, contextPanel.TRegTest + 1];
            double[] NCOM = new double[4 + 1] { 999999f, 4, 6, 4, 1 };
            float[,,,] RESUL = new float[NBSTA + 1, 96 + 1, 4 + 1, contextPanel.NbSeg + 1];
            double[] Z = new double[15 + 1];
            double[] YR = new double[15 + 1];

            int[,] COMPT = new int[contextPanel.NbSeg + 1, contextPanel.TRegTest + 1];
            int[] ITPO = new int[contextPanel.TRegTest + 1];
            int[] ITTP = new int[contextPanel.TRegTest + 1];
            int[] IZAP = new int[contextPanel.TRegTest + 1];
            int[] IPPS = new int[contextPanel.TRegTest + 1];
            int[] ITJP = new int[contextPanel.TRegTest + 1];
            int[,] SEGM = new int[contextPanel.NbSeg + 1, contextPanel.TRegTest + 1];
            int[] NB = new int[contextPanel.TRegTest + 1];
            int[] ITTL = new int[contextPanel.TRegTest + 1];
            int[] ITOU = new int[contextPanel.TRegTest + 1];
            int[] COMB = new int[15 + 1];
            int[] IZAB = new int[contextPanel.TRegTest + 1];
            int[] ITJR = new int[contextPanel.TRegTest + 1];
            int[] IM = new int[contextPanel.TRegTest + 1];
            //int[,] POIQH = new int[96 + 1, NBJOUR + 1];
            int[,,] PATRON = new int[96 + 1, contextPanel.NbSeg, NBIND + 1];
            int[,,] JATOU = new int[96 + 1, 4 + 1, NBIND + 1];
            int[] TREG = new int[contextPanel.NbSeg + 1];
            double ZR = 0f;
            double UR = 0f;
            double PR = 0f;
            double TAU = 0f;
            StringBuilder sbSortie = new StringBuilder();

            //  id  time
            //  1     05h00 - 06h00 = 4
            //  2     06h00 - 09h00 = 12
            //  3     09h00 - 12h00 = 12
            //  4     12h00 - 14h00 = 8
            //  5     14h00 - 16h00 = 8
            //  6     16h00 - 18h00 = 8
            //  7     18h00 - 20h00 = 8
            //  8     20h00 - 24h00 = 16
            //  9     24h00 - 05h00 = 20

            // OPEN(18, FILE = '#OUTPUT#ZUPTAUSA',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            FileStream writeStream = new FileStream(pathZuptausa, FileMode.Create);
            BinaryWriter writeBinaryZuptausa = new BinaryWriter(writeStream);

            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
                sbSortie.AppendLine("Station " + NOP.ToString());

                //??  WRITE(16, 11) NOP

                // INITIALISATIONS
                int IP = NOP;
                int IPO = 6 * (IP - 1) + 1;
                int NOSH = NOP;

                // BOUCLE INDIVIDUS
                // 30
                int IG = 0;
                foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)
                {
                    IG = IG + 1;

                    // BOUCLE 1 / 4h
                    for (int IQ = 1; IQ <= 96; IQ++)
                    {
                        if (IQ==24)
                        {

                        }
                        int IH = fushab09Indiv.KHSA[ITH[IQ], NOSH] - 48;
                        if (IH == 0) IH = 5;
                        int NJOU = 0;
                        for (int IJ = 1; IJ <= contextPanel.NbSeg-1; IJ++)
                        {
                            PATRON[IQ, IJ, IG] = 0;
                        }
                        for (int IJ = 1; IJ <= 4; IJ++)
                        {
                            JATOU[IQ, IJ, IG] = 0;
                        }
                        for (int I = 1; I <= contextPanel.NbSeg; I++)
                        {
                            TREG[I] = regrs[2, IP, IQ, I];
                        }

                        if (TREG[1] == contextPanel.TRegTest) continue;
                        int IJK = 1;
                        for (int I = 1; I <= 22; I += 7)
                        {
                            ushort[] bits = new ushort[7];
                            for (int b = 1; b <= 6; b++)
                                bits[b] = JN[I][IG].VSor[b, NOP];

                            if (L1BITFCT(bits, IQ))
                            {
                                PATRON[IQ, IJK, IG] = 1;
                                NJOU = NJOU + 1;
                            }
                            IJK = IJK + 1;
                        }
                        if (NJOU == 0) JATOU[IQ, 1, IG] = 1;
                        if (NJOU == 0 && IH == 5) JATOU[IQ, 2, IG] = 1;
                        if (NJOU == 4) JATOU[IQ, 3, IG] = 1;
                        if (NJOU == 4 && IH == 1) JATOU[IQ, 4, IG] = 1;
                    } //  for (int IQ = 1; IQ <= 96; IQ++)
                } // foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)

                // 120 
                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    if (IQ == 24)
                    {

                    }
                    sbSortie.AppendLine($"IQ = {IQ}");
                    Console.WriteLine($"--------STATION {NOP} --------1/4h {IQ} ");
                    for (int I = 1; I <= contextPanel.NbSeg; I++)
                    {
                        TREG[I] = regrs[2, IP, IQ, I];
                    }

                    if (TREG[1] != contextPanel.TRegTest)
                    {
                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            int N2 = contextPanel.SEG1[N1];
                            int N3 = contextPanel.SEG2[N2];
                            int N4 = (contextPanel.Enquete != Enquete.PanelIleDeFrance) ? contextPanel.SEG3[N3] : 0;
                            NB[1] = N1;
                            NB[2] = N2;
                            NB[3] = N3;
                            NB[4] = (contextPanel.Enquete != Enquete.PanelIleDeFrance) ? N4 : 1;
                            if (contextPanel.Enquete != Enquete.PanelIleDeFrance) NB[5] = 1;
                            int NIV = TREG[N1] + 1;
                            if (((N1 == 1) || (NIV == 1))
                                ||
                               (!((NIV == 2 && contextPanel.SEG1[N1] == contextPanel.SEG1[N1 - 1]) ||
                                  (NIV == 3 && contextPanel.SEG2[contextPanel.SEG1[N1]] == contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]) ||
                                  (contextPanel.Enquete != Enquete.PanelIleDeFrance && (NIV == 4) && contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]] == contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]]) ||
                                  (NIV == contextPanel.TRegTest))))
                            {

                                for (int I = 1; I <= contextPanel.NbSeg; I++)
                                {
                                    for (int J = 1; J <= contextPanel.TRegTest; J++)
                                    {
                                        SEGM[I, J] = 0;
                                        COMPT[I, J] = 0;
                                        if (I <= 4) C[I, J] = 0f;
                                    }
                                }

                                for (int J = 1; J <= contextPanel.TRegTest; J++)
                                {
                                    IM[J] = 0;
                                    IPPS[J] = 0;
                                    ITOU[J] = 0;
                                    ITPO[J] = 0;
                                    ITTL[J] = 0;
                                    ITTP[J] = 0;
                                    IZAB[J] = 0;
                                    IZAP[J] = 0;
                                    ITJR[J] = 0;
                                    ITJP[J] = 0;
                                }

                                // BOUCLE INDIVIDUS
                                for (IG = 1; IG <= NBIND; IG++)
                                {
                                    bool KHI2_IG_NIV_EQ_NB_NIV = (NIV == contextPanel.TRegTest && NB[NIV] == 1) || (KHI2[IG, NIV + 1] == NB[NIV]);
                                    int IPERS = KHI2[IG, 1];

                                    IPERS = IPERS * 10;
                                    if (KHI2_IG_NIV_EQ_NB_NIV) IM[NIV] = IM[NIV] + 1;
                                    if (KHI2_IG_NIV_EQ_NB_NIV) IPPS[NIV] = IPPS[NIV] + IPERS;
                                    int IAUD = 0;
                                    int NJOU = 0;
                                    for (int I = 1; I <= contextPanel.NbSeg-1; I++)
                                    {
                                        COMB[I] = PATRON[IQ, I, IG];
                                        if (COMB[I] == 1)
                                        {
                                            IAUD = 1;
                                            NJOU = NJOU + 1;
                                        }
                                    }
                                    if (IAUD == 0)
                                    {
                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            COMPT[1, NIV] = COMPT[1, NIV] + IPERS;
                                            SEGM[1, NIV] = SEGM[1, NIV] + 1;
                                            if (JATOU[IQ, 2, IG] == 1) IZAB[NIV] = IZAB[NIV] + 1;
                                            if (JATOU[IQ, 2, IG] == 1) IZAP[NIV] = IZAP[NIV] + IPERS;
                                        }
                                    }
                                    if (IAUD == 1)
                                    {
                                        int IJ = NJOU + 1;
                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            COMPT[IJ, NIV] = COMPT[IJ, NIV] + IPERS;
                                            SEGM[IJ, NIV] = SEGM[IJ, NIV] + 1;

                                            ITOU[NIV] = ITOU[NIV] + 1;
                                            ITPO[NIV] = ITPO[NIV] + IPERS;
                                        }
                                        if (NJOU == 4)
                                        {
                                            if (KHI2_IG_NIV_EQ_NB_NIV)
                                            {
                                                ITTL[NIV] = ITTL[NIV] + 1;
                                                ITTP[NIV] = ITTP[NIV] + IPERS;
                                                if (JATOU[IQ, 4, IG] == 1)
                                                {
                                                    ITJR[NIV] = ITJR[NIV] + 1;
                                                    ITJP[NIV] = ITJP[NIV] + IPERS;
                                                }
                                            }
                                        }

                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            #region CALCUL DES MOYENNES

                                            // PREMIER JOUR
                                            float TOT = 0f;
                                            for (int I01 = 1; I01 <= 4; I01++)
                                                if (COMB[I01] == 1) TOT += IPERS;
                                            C[1, NIV] += TOT;

                                            // 2eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 3; I01++)
                                                for (int I02 = I01 + 1; I02 <= 4; I02++)
                                                    if (COMB[I01] == 1 || COMB[I02] == 1) TOT += IPERS;
                                            C[2, NIV] += TOT;

                                            // 3eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 2; I01++)
                                                for (int I02 = I01 + 1; I02 <= 3; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 4; I03++)
                                                        if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1) TOT += IPERS;
                                            C[3, NIV] += TOT;

                                            // 4eme JOUR
                                            TOT = 0f;
                                            if (COMB[1] == 1 || COMB[2] == 1 || COMB[3] == 1 || COMB[4] == 1) TOT += IPERS;
                                            C[4, NIV] += TOT;

                                            #endregion Calcul des mioyennes
                                        }
                                    } // FIN BOUCLE INDIVIDUS
                                } // FOR IG

                                // IMPRESSION DES RESULTATS
                                int IN = TREG[N1] + 1;
                                double POPS = 0d;
                                for (int I = 1; I <= NBIND; I++)
                                {
                                    if ((IN == contextPanel.TRegTest && NB[IN] == 1) || (KHI2[I, IN + 1] == NB[IN])) POPS = POPS + KHI2[I, 1];
                                }
                                POPS = POPS * 10d;
                                double DIRAC0 = COMPT[1, IN] / POPS;
                                double DIRAC1 = ITTP[IN] / POPS;
                                double PC1 = IZAP[IN] / POPS;
                                int IPOP = 0;
                                for (int I = 2; I <= contextPanel.NbSeg; I++)
                                {
                                    IPOP = IPOP + COMPT[I, IN] * (I - 1);
                                }

                                for (int I = 2; I <= contextPanel.NbSeg; I++)
                                {
                                    if (I != contextPanel.NbSeg) SEGM[I + 1, IN] += SEGM[I, IN];
                                    if (I != contextPanel.NbSeg) COMPT[I + 1, IN] += COMPT[I, IN];
                                }

                                double GRP = IPOP * 100;
                                GRP = GRP / POPS / 4d;
                                PR = (IPOP - ITTP[IN] * 4d) / 3d;
                                if (POPS == (COMPT[1, IN] + ITTP[IN]))
                                {
                                    ZR = COMPT[1, IN] / POPS;
                                    UR = ITTP[IN] / POPS;
                                    PR = 1f;
                                    TAU = 0f;
                                }
                                else
                                {
                                    PR = PR / (POPS - COMPT[1, IN] - ITTP[IN]);
                                    for (int I = 1; I <= 4; I++)
                                    {
                                        Z[I] = 1d * C[I, IN] / NCOM[I] / POPS * 100d;
                                    }

                                    // APPEL DE LA FONCTION DIFFERENCE

                                    GRP = GRP / 100d;
                                    double ZA = DIRAC0;
                                    ZR = IZAP[IN] / POPS;
                                    double UA = ITJP[IN] / POPS;
                                    UR = DIRAC1;

                                    //WRITE(16, *) GRP, ZA, ZR, UA, UR
                                    sbSortie.AppendLine($"{GRP}  {ZA}  {ZR}  {UA}  {UR} {PR} {POPS} {COMPT[1, IN]} {ITTP[IN]}");

                                    float ND = 4;
                                    MINIMISE(GRP, Z, ref PR, ref UR, ref ZR, out TAU, ZA, UA, ND, qhps: true); // float GRP, float[] ZC, ref float PR, ref float UR, ref float ZR, out float TAU, float ZA, float UA, float NB)
                                }
                            } // if (((N1 == 1) || (NIV == 1) ) 
                            RESUL[NOP, IQ, 1, N1] = Convert.ToSingle(ZR);
                            RESUL[NOP, IQ, 2, N1] = Convert.ToSingle(UR);
                            RESUL[NOP, IQ, 3, N1] = Convert.ToSingle(PR);
                            RESUL[NOP, IQ, 4, N1] = Convert.ToSingle(TAU);

                            sbSortie.AppendLine($"CELLULE {N1} Z {ZR} U {UR} P {PR} TAU {TAU}");

                        } // for (int N1 = 1; N1 <= 16; N1++)
                    }
                    else
                    {
                        sbSortie.AppendLine(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            RESUL[NOP, IQ, 1, N1] = 1f;
                            RESUL[NOP, IQ, 2, N1] = 0f;
                            RESUL[NOP, IQ, 3, N1] = 0f;
                            RESUL[NOP, IQ, 4, N1] = 0f;
                        }
                    }
                    // FIN DE TRAITEMENT CELLULE
                    // 1000 
                    for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                    {
                        for (int I = 1; I <= 4; I++)
                        {
                            writeBinaryZuptausa.Write(RESUL[NOP, IQ, I, N1]);
                        }
                    }
                } // for (int IQ = 1; IQ <= 96; IQ++)
                  // 1001 
                  // 900  
            } // for (int NOP = 1; NOP <= NBSTA; NOP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");

            if (File.Exists(pathSortie)) File.Delete(pathSortie);
            File.AppendAllText(pathSortie, sbSortie.ToString());

            //FORMAT('CELLULE ',I2,' Z=',F9.7,' U=',F9.7,' P=',F9.7,' TAU=',F9.7) == ??? 
            //? Console.WriteLine($"CELLULE {I2} Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7)


            writeBinaryZuptausa.Close();
            writeStream.Close();

            return RESUL;
        }

        private float[,,,] sav1qhpd(ContextPanel contextPanel, int NBIND, int NBSTA, byte[,,,] regrs, int[,] KHI2, List<Fushab09Indiv> fushab09Indivs, VsorPoid[][][] JNByWeek, VsorPoid[][] JN, string pathZuptaudi, string pathSortie)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES PARAMETRES z,u,p,tau
            // SAMEDI
            // VERSION 2
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;

            double[,] C = new double[15 + 1, contextPanel.TRegTest + 1];
            double[] NCOM = new double[4 + 1] { 999999f, 4, 6, 4, 1 };
            float[,,,] RESUL = new float[NBSTA + 1, 96 + 1, 4 + 1, contextPanel.NbSeg + 1];
            double[] Z = new double[15 + 1];
            double[] YR = new double[15 + 1];

            int[,] COMPT = new int[contextPanel.NbSeg + 1, contextPanel.TRegTest + 1];
            int[] ITPO = new int[contextPanel.TRegTest + 1];
            int[] ITTP = new int[contextPanel.TRegTest + 1];
            int[] IZAP = new int[contextPanel.TRegTest + 1];
            int[] IPPS = new int[contextPanel.TRegTest + 1];
            int[] ITJP = new int[contextPanel.TRegTest + 1];
            int[,] SEGM = new int[contextPanel.NbSeg + 1, contextPanel.TRegTest + 1];
            int[] NB = new int[contextPanel.TRegTest + 1];
            int[] ITTL = new int[contextPanel.TRegTest + 1];
            int[] ITOU = new int[contextPanel.TRegTest + 1];
            int[] COMB = new int[15 + 1];
            int[] IZAB = new int[contextPanel.TRegTest + 1];
            int[] ITJR = new int[contextPanel.TRegTest + 1];
            int[] IM = new int[contextPanel.TRegTest + 1];
            //int[,] POIQH = new int[96 + 1, NBJOUR + 1];
            int[,,] PATRON = new int[96 + 1, contextPanel.NbSeg, NBIND + 1];
            int[,,] JATOU = new int[96 + 1, 4 + 1, NBIND + 1];
            int[] TREG = new int[contextPanel.NbSeg + 1];
            double ZR = 0f;
            double UR = 0f;
            double PR = 0f;
            double TAU = 0f;

            StringBuilder sbSortie = new StringBuilder();

            //  id  time
            //  1     05h00 - 06h00 = 4
            //  2     06h00 - 09h00 = 12
            //  3     09h00 - 12h00 = 12
            //  4     12h00 - 14h00 = 8
            //  5     14h00 - 16h00 = 8
            //  6     16h00 - 18h00 = 8
            //  7     18h00 - 20h00 = 8
            //  8     20h00 - 24h00 = 16
            //  9     24h00 - 05h00 = 20


            // OPEN(18, FILE = '#OUTPUT#ZUPTAUSA',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            FileStream writeStream = new FileStream(pathZuptaudi, FileMode.Create);
            BinaryWriter writeBinaryZuptaudi = new BinaryWriter(writeStream);

            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
                sbSortie.AppendLine("Station " + NOP.ToString());

                // INITIALISATIONS
                int IP = NOP;
                int IPO = 6 * (IP - 1) + 1;
                int NOSH = NOP;

                // BOUCLE INDIVIDUS
                // 30
                int IG = 0;
                foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)
                {
                    IG = IG + 1;

                    // BOUCLE 1 / 4h
                    for (int IQ = 1; IQ <= 96; IQ++)
                    {
                        int IH = fushab09Indiv.KHDI[ITH[IQ], NOSH] - 48;
                        if (IH == 0) IH = 5;
                        int NJOU = 0;
                        for (int IJ = 1; IJ <= 4; IJ++)
                        {
                            PATRON[IQ, IJ, IG] = 0;
                        }
                        for (int IJ = 1; IJ <= 4; IJ++)
                        {
                            JATOU[IQ, IJ, IG] = 0;
                        }
                        for (int I = 1; I <= contextPanel.NbSeg; I++)
                        {
                            TREG[I] = regrs[3, IP, IQ, I];
                        }

                        if (TREG[1] == contextPanel.TRegTest) continue;
                        int IJK = 1;
                        for (int I = 2; I <= 23; I += 7)
                        {
                            ushort[] bits = new ushort[7];
                            for (int b = 1; b <= 6; b++)
                                bits[b] = JN[I][IG].VSor[b, NOP];

                            if (L1BITFCT(bits, IQ))
                            {
                                PATRON[IQ, IJK, IG] = 1;
                                NJOU = NJOU + 1;
                            }
                            IJK = IJK + 1;
                        }
                        if (NJOU == 0) JATOU[IQ, 1, IG] = 1;
                        if (NJOU == 0 && IH == 5) JATOU[IQ, 2, IG] = 1;
                        if (NJOU == 4) JATOU[IQ, 3, IG] = 1;
                        if (NJOU == 4 && IH == 1) JATOU[IQ, 4, IG] = 1;
                    } //  for (int IQ = 1; IQ <= 96; IQ++)
                } // foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)

                // 120 
                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    sbSortie.AppendLine($"IQ = {IQ}");
                    Console.WriteLine($"--------STATION {NOP} --------1/4h {IQ} ");
                    for (int I = 1; I <= contextPanel.NbSeg; I++)
                    {
                        TREG[I] = regrs[3, IP, IQ, I];
                    }

                    if (TREG[1] != contextPanel.TRegTest)
                    {
                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            int N2 = contextPanel.SEG1[N1];
                            int N3 = contextPanel.SEG2[N2];
                            int N4 = (contextPanel.Enquete != Enquete.PanelIleDeFrance) ? contextPanel.SEG3[N3] : 0;
                            NB[1] = N1;
                            NB[2] = N2;
                            NB[3] = N3;
                            NB[4] = (contextPanel.Enquete != Enquete.PanelIleDeFrance) ? N4 : 1;
                            if (contextPanel.Enquete != Enquete.PanelIleDeFrance) NB[5] = 1;
                            int NIV = TREG[N1] + 1;
                            if (((N1 == 1) || (NIV == 1))
                                ||
                               (!((NIV == 2 && contextPanel.SEG1[N1] == contextPanel.SEG1[N1 - 1]) ||
                                  (NIV == 3 && contextPanel.SEG2[contextPanel.SEG1[N1]] == contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]) ||
                                  (contextPanel.Enquete != Enquete.PanelIleDeFrance && NIV == 4 && contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]] == contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]]) ||
                                  (NIV == contextPanel.TRegTest))))
                            {

                                for (int I = 1; I <= contextPanel.NbSeg; I++)
                                {
                                    for (int J = 1; J <= contextPanel.TRegTest; J++)
                                    {
                                        SEGM[I, J] = 0;
                                        COMPT[I, J] = 0;
                                        if (I <= 4) C[I, J] = 0f;
                                    }
                                }

                                for (int J = 1; J <= contextPanel.TRegTest; J++)
                                {
                                    IM[J] = 0;
                                    IPPS[J] = 0;
                                    ITOU[J] = 0;
                                    ITPO[J] = 0;
                                    ITTL[J] = 0;
                                    ITTP[J] = 0;
                                    IZAB[J] = 0;
                                    IZAP[J] = 0;
                                    ITJR[J] = 0;
                                    ITJP[J] = 0;
                                }

                                // BOUCLE INDIVIDUS
                                for (IG = 1; IG <= NBIND; IG++)
                                {
                                    bool KHI2_IG_NIV_EQ_NB_NIV = (NIV == contextPanel.TRegTest && NB[NIV] == 1) || (KHI2[IG, NIV + 1] == NB[NIV]);
                                    int IPERS = KHI2[IG, 1];

                                    IPERS = IPERS * 10;
                                    if (KHI2_IG_NIV_EQ_NB_NIV)
                                    {
                                        IM[NIV] = IM[NIV] + 1;
                                        IPPS[NIV] = IPPS[NIV] + IPERS;
                                    }
                                    int IAUD = 0;
                                    int NJOU = 0;
                                    for (int I = 1; I <= 4; I++)
                                    {
                                        COMB[I] = PATRON[IQ, I, IG];
                                        if (COMB[I] == 1)
                                        {
                                            IAUD = 1;
                                            NJOU = NJOU + 1;
                                        }
                                    }
                                    if (IAUD == 0)
                                    {
                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            COMPT[1, NIV] = COMPT[1, NIV] + IPERS;
                                            SEGM[1, NIV] = SEGM[1, NIV] + 1;
                                            if (JATOU[IQ, 2, IG] == 1) IZAB[NIV] = IZAB[NIV] + 1;
                                            if (JATOU[IQ, 2, IG] == 1) IZAP[NIV] = IZAP[NIV] + IPERS;
                                        }
                                    }
                                    if (IAUD == 1)
                                    {
                                        int IJ = NJOU + 1;
                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            COMPT[IJ, NIV] = COMPT[IJ, NIV] + IPERS;
                                            SEGM[IJ, NIV] = SEGM[IJ, NIV] + 1;

                                            ITOU[NIV] = ITOU[NIV] + 1;
                                            ITPO[NIV] = ITPO[NIV] + IPERS;
                                        }
                                        if (NJOU == 4)
                                        {
                                            if (KHI2_IG_NIV_EQ_NB_NIV)
                                            {
                                                ITTL[NIV] = ITTL[NIV] + 1;
                                                ITTP[NIV] = ITTP[NIV] + IPERS;
                                                if (JATOU[IQ, 4, IG] == 1)
                                                {
                                                    ITJR[NIV] = ITJR[NIV] + 1;
                                                    ITJP[NIV] = ITJP[NIV] + IPERS;
                                                }
                                            }
                                        }

                                        if (KHI2_IG_NIV_EQ_NB_NIV)
                                        {
                                            #region CALCUL DES MOYENNES

                                            // PREMIER JOUR
                                            float TOT = 0f;
                                            for (int I01 = 1; I01 <= 4; I01++)
                                                if (COMB[I01] == 1) TOT += IPERS;
                                            C[1, NIV] += TOT;

                                            // 2eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 3; I01++)
                                                for (int I02 = I01 + 1; I02 <= 4; I02++)
                                                    if (COMB[I01] == 1 || COMB[I02] == 1) TOT += IPERS;
                                            C[2, NIV] += TOT;

                                            // 3eme JOUR
                                            TOT = 0f;
                                            for (int I01 = 1; I01 <= 2; I01++)
                                                for (int I02 = I01 + 1; I02 <= 3; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 4; I03++)
                                                        if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1) TOT += IPERS;
                                            C[3, NIV] += TOT;

                                            // 4eme JOUR
                                            TOT = 0f;
                                            if (COMB[1] == 1 || COMB[2] == 1 || COMB[3] == 1 || COMB[4] == 1) TOT += IPERS;
                                            C[4, NIV] += TOT;

                                            #endregion Calcul des mioyennes
                                        }
                                    } // FIN BOUCLE INDIVIDUS
                                } // FOR IG

                                // IMPRESSION DES RESULTATS
                                int IN = TREG[N1] + 1;
                                double POPS = 0d;
                                for (int I = 1; I <= NBIND; I++)
                                {
                                    if ((IN == contextPanel.TRegTest && NB[IN] == 1) || (KHI2[I, IN + 1] == NB[IN])) POPS = POPS + KHI2[I, 1];
                                }
                                POPS = POPS * 10d;
                                double DIRAC0 = COMPT[1, IN] / POPS;
                                double DIRAC1 = ITTP[IN] / POPS;
                                double PC1 = IZAP[IN] / POPS;
                                int IPOP = 0;
                                for (int I = 2; I <= 5; I++)
                                {
                                    IPOP = IPOP + COMPT[I, IN] * (I - 1);
                                }

                                for (int I = 2; I <= 5; I++)
                                {
                                    if (I != 5) SEGM[I + 1, IN] += SEGM[I, IN];
                                    if (I != 5) COMPT[I + 1, IN] += COMPT[I, IN];
                                }

                                double GRP = IPOP * 100;
                                GRP = GRP / POPS / 4d;
                                PR = (IPOP - ITTP[IN] * 4d) / 3d;
                                if (POPS == (COMPT[1, IN] + ITTP[IN]))
                                {
                                    ZR = COMPT[1, IN] / POPS;
                                    UR = ITTP[IN] / POPS;
                                    PR = 1f;
                                    TAU = 0f;
                                }
                                else
                                {
                                    PR = PR / (POPS - COMPT[1, IN] - ITTP[IN]);
                                    for (int I = 1; I <= 4; I++)
                                    {
                                        Z[I] = 1d * C[I, IN] / NCOM[I] / POPS * 100d;
                                    }

                                    // APPEL DE LA FONCTION DIFFERENCE

                                    GRP = GRP / 100f;
                                    double ZA = DIRAC0;
                                    ZR = IZAP[IN] / POPS;
                                    double UA = ITJP[IN] / POPS;
                                    UR = DIRAC1;

                                    //WRITE(16, *) GRP, ZA, ZR, UA, UR
                                    sbSortie.AppendLine($"{GRP}  {ZA}  {ZR}  {UA}  {UR}");

                                    float ND = 4;
                                    MINIMISE(GRP, Z, ref PR, ref UR, ref ZR, out TAU, ZA, UA, ND); // float GRP, float[] ZC, ref float PR, ref float UR, ref float ZR, out float TAU, float ZA, float UA, float NB)
                                }
                            } // if (((N1 == 1) || (NIV == 1) ) 
                            RESUL[NOP, IQ, 1, N1] = Convert.ToSingle(ZR);
                            RESUL[NOP, IQ, 2, N1] = Convert.ToSingle(UR);
                            RESUL[NOP, IQ, 3, N1] = Convert.ToSingle(PR);
                            RESUL[NOP, IQ, 4, N1] = Convert.ToSingle(TAU);

                            sbSortie.AppendLine($"CELLULE {N1} Z {ZR} U {UR} P {PR} TAU {TAU}");

                        } // for (int N1 = 1; N1 <= 16; N1++)
                    } // if (TREG[1] != 5)
                    else
                    {
                        sbSortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            RESUL[NOP, IQ, 1, N1] = 1f;
                            RESUL[NOP, IQ, 2, N1] = 0f;
                            RESUL[NOP, IQ, 3, N1] = 0f;
                            RESUL[NOP, IQ, 4, N1] = 0f;
                        }
                    }
                    for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                    {
                        for (int I = 1; I <= 4; I++)
                        {
                            writeBinaryZuptaudi.Write(RESUL[NOP, IQ, I, N1]);
                        }
                    }
                } // for (int IQ = 1; IQ <= 96; IQ++)

            } // for (int NOP = 1; NOP <= NBSTA; NOP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");

            if (File.Exists(pathSortie)) File.Delete(pathSortie);
            File.AppendAllText(pathSortie, sbSortie.ToString());

            //FORMAT('CELLULE ',I2,' Z=',F9.7,' U=',F9.7,' P=',F9.7,' TAU=',F9.7) == ??? 
            //? Console.WriteLine($"CELLULE {I2} Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7)

            writeBinaryZuptaudi.Close();
            writeStream.Close();

            return RESUL;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="PathGRPWave"> Chemin pour accÃ¨der au fichier de la 126000</param>
        /// <param name="NbStation"> Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)</param>
        /// <param name="NbGRPModulation">Nombre de modalitÃ©s dans le fichier de GRP 89(Attention + 2 pour le vecteur de poids et un vecteur vide dans le fichier U109)</param>
        /// <param name="NbGRPStation">Nombre de station dans la fichier de GRP U109</param>
        /// <param name="NotorieteStation"></param>
        /// <returns></returns>
        private float[,,,] cgrp75br(ContextPanel contextPanel, string PathGRPWave,
            int NbStation, int NbGRPModulation, int NbGRPStation, int[] ISTA, string pathSortie8, string pathNOUVOGRP)
        {
            if (NbStation == 0
                || NbGRPModulation == 0
                || NbGRPStation == 0)
                return null;

            FileInfo fi = new FileInfo(PathGRPWave);
            if (fi.Length % (NbGRPModulation * 2 + NbGRPStation * 96) != 0)
            {
                throw new Exception($"cgrp75br : taille du fichier d'entree {PathGRPWave} incorrecte NbGRPModulation {NbGRPModulation} NbGRPStation {NbGRPStation}");
            };

            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES GRP 75000 POUR CALAGE

            float[,,,] COUV = new float[37 + 1, 3 + 1, NbStation + 1, 96 + 1];
            int[,] IPOP = new int[37 + 1, 3 + 1];

            int[] KHI2 = new int[NbGRPModulation + 1];
            //int[] ISTA = new int[NbStation + 1];
            int ICSP, IREG, ISEX, IAGE, ISEG, COMPT;
            int[,] ZLEC = new int[96 + 1, NbGRPStation + 1];

            //
            // INITIALISATIONS
            //
            int IG = 0;

            //
            //                              OUVERTURE FICHIER
            //
            FileStream fs = File.Open(PathGRPWave, FileMode.Open);
            fs.Seek(0, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fs);

            COMPT = 0;
            IG = 0;

            while (fs.Position != fs.Length)
            {
                // BOUCLE INDIVIDUS
                for (int i = 1; i <= NbGRPModulation; i++)
                    KHI2[i] = br.ReadUInt16();

                for (int j = 1; j <= NbGRPStation; j++)
                    for (int i = 1; i <= 96; i++)
                        ZLEC[i, j] = br.ReadByte();

                int IPERS = KHI2[1];
                if (IPERS > 0)
                {

                    IG = IG + 1;
                    Console.WriteLine(IG);

                    int IU = 1;
                    if (KHI2[2] == 6) IU = 2;
                    if (KHI2[2] == 7) IU = 3;

                    // DETERMINATION DU SEGMENT 2019
                    //int IND_CSP = 5 + 2;
                    //int IND_AGE = 20 + 2;
                    //int IND_SEX = 3;
                    //int IND_REG = 13 + 2;

                    // DETERMINATION DU SEGMENT 2020 !!!!!! TODO:
                    //int IND_CSP = 4 + 2;
                    //int IND_AGE = 18 + 2;
                    //int IND_SEX = 3;
                    //int IND_REG = 12 + 2;

                    ICSP = 1;
                    if (KHI2[contextPanel.IND_CSP] == 1 || KHI2[contextPanel.IND_CSP] > 4) ICSP = 2;
                    if (KHI2[contextPanel.IND_CSP] > 6) ICSP = 3;
                    ISEX = KHI2[contextPanel.IND_SEX];
                    if (ISEX > 2) ISEX = 2;
                    IAGE = 1; // < 35
                    if (contextPanel.Age35a59.Contains(KHI2[contextPanel.IND_AGE])) IAGE = 2;// >= 35     <= 59
                    if (contextPanel.Age60Plus.Contains(KHI2[contextPanel.IND_AGE])) IAGE = 3; // >=60
                    IREG = 1;
                    if (KHI2[contextPanel.IND_REG] > 6) IREG = 2;

                    if (IAGE == 3)
                    {
                        // 70
                        ISEG = 13;
                        if (IREG == 2) ISEG = 14;
                        if (ISEX == 2) ISEG = ISEG + 2;
                    }
                    else if (ICSP != 1)
                    {
                        if (ICSP == 3)
                        {
                            if (IAGE == 2)
                            {
                                ISEG = 12;
                            }
                            else
                            {
                                ISEG = 8;
                                if (IREG == 1 && ISEX == 1) ISEG = 6;
                                if (IREG == 1 && ISEX == 2) ISEG = 7;
                            }
                        }
                        else if (IAGE == 2)
                        {
                            ISEG = 11;
                            if (IREG == 1 && ISEX == 1) ISEG = 9;
                            if (IREG == 1 && ISEX == 2) ISEG = 10;
                        }
                        else
                        {
                            ISEG = 4;
                            if (ISEX == 2) ISEG = 5;
                        }
                    }
                    else
                    {
                        ISEG = 3;
                        if (IREG == 1 && ISEX == 1) ISEG = 1;
                        if (IREG == 1 && ISEX == 2) ISEG = 2;
                    }


                    if (ISEG < 1 || ISEG > 16)
                    {
                        return null;
                    }
                    else
                    {
                        int ISE2 = 16 + contextPanel.SEG1[ISEG];
                        int ISE3 = 26 + contextPanel.SEG2[contextPanel.SEG1[ISEG]];
                        int ISE4 = 32 + contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[ISEG]]];

                        IPOP[ISEG, IU] += IPERS;
                        IPOP[ISE2, IU] += IPERS;
                        IPOP[ISE3, IU] += IPERS;
                        IPOP[ISE4, IU] += IPERS;
                        IPOP[37, IU] += IPERS;

                        // BOUCLE STATIONS
                        for (int IS = 1; IS <= NbStation; IS++)
                        {
                            int NS = ISTA[IS];

                            if (NS != 0)
                            {
                                //                              BOUCLE 1 / 4h
                                for (int IQ = 1; IQ <= 96; IQ++)
                                {
                                    if (ZLEC[IQ, NS] > 0)
                                    {
                                        COUV[ISEG, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];

                                        COUV[ISE2, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];

                                        COUV[ISE3, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];

                                        COUV[ISE4, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];

                                        COUV[37, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];
                                    }
                                }
                            }
                        }
                    }

                    if (ISEG == 2) COMPT = COMPT + 1;
                }
            }
            br.Close();
            fs.Close();

            if (File.Exists(pathSortie8)) File.Delete(pathSortie8);
            if (File.Exists(pathNOUVOGRP)) File.Delete(pathNOUVOGRP);
            StringBuilder sbSortie8 = new StringBuilder();

            sbSortie8.AppendLine(" NBGUS: " + IG.ToString());
            for (int I = 1; I <= 37; I++)
            {
                sbSortie8.Append(I.ToString("000") + " : ");

                for (int J = 1; J <= 3; J++)
                {
                    sbSortie8.Append(IPOP[I, J].ToString("").PadLeft(9) + " ");

                    for (int K = 1; K <= NbStation; K++)
                    {
                        for (int L = 1; L <= 96; L++)
                        {
                            COUV[I, J, K, L] = COUV[I, J, K, L] / 12f / Convert.ToSingle(IPOP[I, J]);
                        }
                    }

                }
                sbSortie8.AppendLine();
            }
            File.AppendAllText(pathSortie8, sbSortie8.ToString());

            using (BinaryWriter writer = new BinaryWriter(File.Open(pathNOUVOGRP, FileMode.Create)))
            {
                for (int L = 1; L <= 96; L++)
                {
                    for (int K = 1; K <= NbStation; K++)
                    {
                        for (int J = 1; J <= 3; J++)
                        {
                            for (int I = 1; I <= 37; I++)
                                writer.Write(COUV[I, J, K, L]);
                        }

                    }
                }
            }



            return COUV;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="PathGRPWave"> Chemin pour accÃ¨der au fichier de la 126000</param>
        /// <param name="NbStation"> Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)</param>
        /// <param name="NbGRPModulation">Nombre de modalitÃ©s dans le fichier de GRP 89(Attention + 2 pour le vecteur de poids et un vecteur vide dans le fichier U109)</param>
        /// <param name="NbGRPStation">Nombre de station dans la fichier de GRP U109</param>
        /// <param name="NotorieteStation"></param>
        /// <returns></returns>
        private float[,,,] cgrp75br_Idf(ContextPanel contextPanel, string PathGRPWave,
            int NbStation, int NbGRPModulation, int NbGRPStation, int[] ISTA, string pathSortie8, string pathNOUVOGRP)
        {
            if (NbStation == 0
                || NbGRPModulation == 0
                || NbGRPStation == 0)
                return null;

            FileInfo fi = new FileInfo(PathGRPWave);
            if (fi.Length % (NbGRPModulation * 2 + NbGRPStation * 96) != 0)
            {
                throw new Exception($"cgrp75br_Idf : taille du fichier d'entree {PathGRPWave} incorrecte NbGRPModulation {NbGRPModulation} NbGRPStation {NbGRPStation}");
            };

            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES GRP 75000 POUR CALAGE

            float[,,,] COUV = new float[11 + 1, 3 + 1, NbStation + 1, 96 + 1];
            int[,] IPOP = new int[11 + 1, 3 + 1];

            int[] KHI2 = new int[NbGRPModulation + 1];
            //int[] ISTA = new int[NbStation + 1];
            int ISEX, IAGE, ISEG = 0, COMPT;
            int[,] ZLEC = new int[96 + 1, NbGRPStation + 1];

            //
            // INITIALISATIONS
            //
            int IG = 0;

            //
            //                              OUVERTURE FICHIER
            //
            FileStream fs = File.Open(PathGRPWave, FileMode.Open);
            fs.Seek(0, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fs);

            COMPT = 0;
            IG = 0;

            while (fs.Position != fs.Length)
            {
                // BOUCLE INDIVIDUS
                for (int i = 1; i <= NbGRPModulation; i++)
                    KHI2[i] = br.ReadUInt16();

                for (int j = 1; j <= NbGRPStation; j++)
                    for (int i = 1; i <= 96; i++)
                        ZLEC[i, j] = br.ReadByte();

                int IPERS = KHI2[1];
                if (IPERS < 0) IPERS = 65536 + IPERS;
                if (IPERS >= 0)
                {
                    IG = IG + 1;
                    Console.WriteLine(IG);

                    int IU = 1;
                    if (KHI2[2] == 6) IU = 2;
                    if (KHI2[2] == 7) IU = 3;

                    ISEX = KHI2[contextPanel.IND_SEX];
                    if (ISEX > 2) ISEX = 2;
                    IAGE = 1; // < 35
                    if (contextPanel.Age35a59.Contains(KHI2[contextPanel.IND_AGE])) IAGE = 2;// >= 35     <= 59
                    if (contextPanel.Age60Plus.Contains(KHI2[contextPanel.IND_AGE])) IAGE = 3; // >=60

                    if ((IAGE == 1) && (ISEX == 1)) ISEG = 1;
                    if ((IAGE == 1) && (ISEX == 2)) ISEG = 2;
                    if ((IAGE == 2) && (ISEX == 1)) ISEG = 3;
                    if ((IAGE == 2) && (ISEX == 2)) ISEG = 4;
                    if (IAGE == 3) ISEG = 5;
                    if (ISEG < 1 || ISEG > 5)
                    {
                        Console.WriteLine($" ISEG= {ISEG}");
                        throw new Exception(" cgrp75 : ISEG= {ISEG} (ISEG < 1 || ISEG > 5)");
                    }
                    else
                    {
                        int ISE2 = 5 + contextPanel.SEG1[ISEG];
                        int ISE3 = 8 + contextPanel.SEG2[contextPanel.SEG1[ISEG]];
                        IPOP[ISEG, IU] += IPERS;
                        IPOP[ISE2, IU] += IPERS;
                        IPOP[ISE3, IU] += IPERS;
                        IPOP[11, IU] += IPERS;

                        // BOUCLE STATIONS
                        for (int IS = 1; IS <= NbStation; IS++)
                        {
                            int NS = ISTA[IS];

                            if (NS != 0)
                            {
                                //                              BOUCLE 1 / 4h
                                for (int IQ = 1; IQ <= 96; IQ++)
                                {
                                    if (ZLEC[IQ, NS] > 0)
                                    {
                                        COUV[ISEG, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];

                                        COUV[ISE2, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];

                                        COUV[ISE3, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];

                                        COUV[11, IU, IS, IQ] += IPERS * ZLEC[IQ, NS];
                                    }
                                }
                            }
                        }
                    }
                    if (ISEG == 2) COMPT = COMPT + 1;
                }
            }
            br.Close();
            fs.Close();

            if (File.Exists(pathSortie8)) File.Delete(pathSortie8);
            if (File.Exists(pathNOUVOGRP)) File.Delete(pathNOUVOGRP);
            StringBuilder sbSortie8 = new StringBuilder();

            sbSortie8.AppendLine(" NBGUS: " + IG.ToString());
            for (int I = 1; I <= 11; I++)
            {
                sbSortie8.Append(I.ToString("000") + " : ");

                for (int J = 1; J <= 3; J++)
                {
                    sbSortie8.Append(IPOP[I, J].ToString("").PadLeft(9) + " ");

                    for (int K = 1; K <= NbStation; K++)
                    {
                        for (int L = 1; L <= 96; L++)
                        {
                            COUV[I, J, K, L] = COUV[I, J, K, L] / 12f / Convert.ToSingle(IPOP[I, J]);
                        }
                    }

                }
                sbSortie8.AppendLine();
            }
            File.AppendAllText(pathSortie8, sbSortie8.ToString());

            using (BinaryWriter writer = new BinaryWriter(File.Open(pathNOUVOGRP, FileMode.Create)))
            {
                for (int L = 1; L <= 96; L++)
                {
                    for (int K = 1; K <= NbStation; K++)
                    {
                        for (int J = 1; J <= 3; J++)
                        {
                            for (int I = 1; I <= 11; I++)
                                writer.Write(COUV[I, J, K, L]);
                        }
                    }
                }
            }

            return COUV;
        }

        private void cont75br(ContextPanel contextPanel, int NBIND, int NBSTA, int popLV, int popS, int popD, float[,,,] Couverture, string pathCouv)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CONTROLE DES GRP 75000 POUR CALAGE
            //
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int[] POP = new int[3 + 1] { 999999, popLV, popS, popD };

            StringBuilder sb = new StringBuilder();
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                sb.AppendLine($"----- STATION {IP} -----");
                for (int I = 1; I <= 96; I++)
                {
                    sb.Append($"{I.ToString("0000")} : ");
                    for (int J = 1; J <= 3; J++)
                    {
                        float val = Couverture[contextPanel.IdxTot, J, IP, I] * POP[J];
                        int iVal = Convert.ToInt32(Math.Truncate((val + 500f) / 1000f));
                        sb.Append($" {iVal.ToString().PadLeft(8)} ");
                    }
                    sb.AppendLine();
                }
            }
            if (File.Exists(pathCouv)) File.Delete(pathCouv);
            File.AppendAllText(pathCouv, sb.ToString());
        }

        // RESUL = ZUPTAUSE
        private float[,,,] cnzuptse(ContextPanel contextPanel, int NBIND, int NBSTA, string pathCnzuptse, string pathZuptauseCor, float[,,,] COUV, byte[,,,] REGRS, float[,,,] RESUL)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALAGE SUR GRP 75000 + ET RECALCUL DES PARAMETRES STATION
            //
            //        LUNDI - VENDREDI
            //
            int IDSUDRAD = 19;

            float[,,,] RESULCOR = new float[NBSTA + 1, 96 + 1, 4 + 1, contextPanel.IdxTot + 1];
            int[] TREG = new int[contextPanel.NbSeg + 1];

            // BOUCLE STATIONS
            StringBuilder sortie = new StringBuilder();
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                sortie.AppendLine($" Station {IP}");
                Console.WriteLine($"--------STATION {IP} --------");

                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    int NQ = IQ + 76;
                    if (NQ > 96) NQ = NQ - 96;
                    sortie.AppendLine($" QH {IQ}");
                    Console.WriteLine($"Traitement 1/4h {IQ}");

                    // AUCUNE AUDIENCE POUR CE 1 / 4h
                    for (int I = 1; I <= contextPanel.NbSeg; I++)
                    {
                        TREG[I] = REGRS[1, IP, IQ, I];
                    }

                    if (TREG[1] != contextPanel.TRegTest)  // 5 ou 4 idf
                    {
                        float GRPA = 0f;
                        float GRPN = 0f;
                        float ZR = 0f;
                        float UR = 0f;
                        float PR = 0f;
                        float TAU = 0f;
                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            int IN = TREG[N1] + 1;
                            if (((N1 == 1) || (IN == 1))
                                ||
                               (!((IN == 2 && contextPanel.SEG1[N1] == contextPanel.SEG1[N1 - 1]) ||
                                  (IN == 3 && contextPanel.SEG2[contextPanel.SEG1[N1]] == contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]) ||
                                  (contextPanel.Enquete != Enquete.PanelIleDeFrance && IN == 4 && contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]] == contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]]) ||
                                  (IN == contextPanel.TRegTest))))
                            {

                                // LECTURE DES Z, U, P, TAU ORIGINAUX
                                //   20
                                ZR = RESUL[IP, IQ, 1, N1];
                                UR = RESUL[IP, IQ, 2, N1];
                                PR = RESUL[IP, IQ, 3, N1];
                                TAU = RESUL[IP, IQ, 4, N1];
                                float VA = 1f - ZR - UR;
                                GRPA = VA * PR + UR;

                                if (IP != IDSUDRAD)
                                {
                                    int ISEG = N1;
                                    if (IN == 2) ISEG = contextPanel.SEGBASE[0] + contextPanel.SEG1[N1];
                                    if (IN == 3) ISEG = contextPanel.SEGBASE[1] + contextPanel.SEG2[contextPanel.SEG1[N1]];
                                    if (IN == 4 && contextPanel.Enquete != Enquete.PanelIleDeFrance) ISEG = contextPanel.SEGBASE[2] + contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]];
                                    if (IN == contextPanel.TRegTest) ISEG = contextPanel.IdxTot;
                                    // CALCUL NOUVEAU P
                                    GRPN = COUV[ISEG, 1, IP, NQ];
                                    bool process = false;
                                    if (GRPA != 0f && GRPN != 0f)
                                    {
                                        float V = 1f - ZR - UR;
                                        if (V <= 0d)
                                        {
                                            UR = GRPN;
                                            ZR = 1f - UR;
                                            process = true;
                                        }
                                        else if (GRPN > 1f - ZR)
                                        {
                                            ZR = 1f - GRPN;
                                            V = 1f - ZR - UR;
                                            PR = GRPN - UR;
                                            if (V > 0) PR = PR / V;
                                            process = true;
                                        }
                                        if (!process)
                                        {
                                            if (GRPN >= GRPA)
                                            {
                                                PR = GRPN - UR;
                                                if (V > 0) PR = PR / V;
                                            }
                                            else
                                            {
                                                UR = UR * GRPN / GRPA;
                                                V = 1f - ZR - UR;
                                                PR = GRPN - UR;
                                                if (V > 0) PR = PR / V;
                                            }
                                        }
                                    }
                                } // if (IP != IDSUDRAD)
                            }
                            // FIN DE TRAITEMENT CELLULE 1000
                            RESULCOR[IP, IQ, 1, N1] = ZR;
                            RESULCOR[IP, IQ, 2, N1] = UR;
                            RESULCOR[IP, IQ, 3, N1] = PR;
                            RESULCOR[IP, IQ, 4, N1] = TAU;

                            sortie.Append("CELLULE " + N1.ToString());
                            sortie.Append(" Z = " + ZR.ToString("0.0000000"));
                            sortie.Append(" U = " + UR.ToString("0.0000000"));
                            sortie.Append(" P = " + PR.ToString("0.0000000"));
                            sortie.Append(" TAU = " + TAU.ToString("0.0000000"));
                            sortie.Append(" GRPA = " + GRPA.ToString("0.0000000"));
                            sortie.AppendLine(" GRPN = " + GRPN.ToString("0.0000000"));

                        } // for (int N1 = 1; N1 <= 16; N1++)

                    }
                    else
                    {
                        sortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            RESULCOR[IP, IQ, 1, N1] = 1f;
                            RESULCOR[IP, IQ, 2, N1] = 0f;
                            RESULCOR[IP, IQ, 3, N1] = 0f;
                            RESULCOR[IP, IQ, 4, N1] = 0f;
                        }
                    }// if (TREG[1] != 5)

                } // for (int IQ = 1; IQ <= 96; IQ++)
            } // for (int IP = 1; IP <= NBSTA; IP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");
            // FORMAT('CELLULE ', I2, ' Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7, ' GRP=', F9.7, ' GRP''=', F9.7)

            if (File.Exists(pathCnzuptse)) File.Delete(pathCnzuptse);
            File.AppendAllText(pathCnzuptse, sortie.ToString());

            // Ecriture fichier sortie
            FileStream writeStream = new FileStream(pathZuptauseCor, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                    {
                        for (int I = 1; I <= 4; I++)
                        {
                            writeBinary.Write(RESULCOR[IP, IQ, I, N1]);
                        }
                    }
                }
            }
            writeBinary.Close();
            writeStream.Close();


            return RESULCOR;
        }

        private float[,,,] cnzuptsa(ContextPanel contextPanel, int NBIND, int NBSTA, string pathCnzuptsa, string pathZuptausaCor, float[,,,] COUV, byte[,,,] REGRS, float[,,,] RESUL)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALAGE SUR GRP 75000 + ET RECALCUL DES PARAMETRES STATION
            //
            //        SAMEDI
            //
            int IDSUDRAD = 19;

            float[,,,] RESULCOR = new float[NBSTA + 1, 96 + 1, 4 + 1, contextPanel.NbSeg + 1];
            int[] TREG = new int[contextPanel.NbSeg + 1];

            // BOUCLE STATIONS
            StringBuilder sortie = new StringBuilder();
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                sortie.AppendLine($" Station {IP}");
                Console.WriteLine($"--------STATION {IP} --------");

                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    int NQ = IQ + 76;
                    if (NQ > 96) NQ = NQ - 96;
                    sortie.AppendLine($" QH {IQ}");
                    Console.WriteLine($"Traitement 1/4h {IQ}");

                    // AUCUNE AUDIENCE POUR CE 1 / 4h
                    for (int I = 1; I <= contextPanel.NbSeg; I++)
                    {
                        TREG[I] = REGRS[2, IP, IQ, I];
                    }

                    if (TREG[1] != contextPanel.TRegTest)
                    {
                        float GRPA = 0f;
                        float GRPN = 0f;
                        float ZR = 0f;
                        float UR = 0f;
                        float PR = 0f;
                        float TAU = 0f;
                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            int IN = TREG[N1] + 1;
                            if (((N1 == 1) || (IN == 1))
                                ||
                               (!((IN == 2 && contextPanel.SEG1[N1] == contextPanel.SEG1[N1 - 1]) ||
                                  (IN == 3 && contextPanel.SEG2[contextPanel.SEG1[N1]] == contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]) ||
                                  (contextPanel.Enquete != Enquete.PanelIleDeFrance && IN == 4 && contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]] == contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]]) ||
                                  (IN == contextPanel.TRegTest))))
                            {

                                // LECTURE DES Z, U, P, TAU ORIGINAUX
                                //   20
                                ZR = RESUL[IP, IQ, 1, N1];
                                UR = RESUL[IP, IQ, 2, N1];
                                PR = RESUL[IP, IQ, 3, N1];
                                TAU = RESUL[IP, IQ, 4, N1];
                                GRPA = (1f - ZR - UR) * PR + UR;

                                if (IP != IDSUDRAD)
                                {
                                    int ISEG = N1;
                                    if (IN == 2) ISEG = contextPanel.SEGBASE[0] + contextPanel.SEG1[N1];
                                    if (IN == 3) ISEG = contextPanel.SEGBASE[1] + contextPanel.SEG2[contextPanel.SEG1[N1]];
                                    if (contextPanel.Enquete != Enquete.PanelIleDeFrance && IN == 4) ISEG = contextPanel.SEGBASE[2] + contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]];
                                    if (IN == contextPanel.TRegTest) ISEG = contextPanel.IdxTot;
                                    // CALCUL NOUVEAU P
                                    GRPN = COUV[ISEG, 2, IP, NQ];
                                    bool process = false;
                                    if (GRPA != 0d && GRPN != 0d)
                                    {
                                        float V = 1f - ZR - UR;
                                        if (V <= 0d)
                                        {
                                            UR = GRPN;
                                            ZR = 1f - UR;
                                            process = true;
                                        }
                                        else if (GRPN > 1f - ZR)
                                        {
                                            ZR = 1f - GRPN;
                                            V = 1f - ZR - UR;
                                            PR = GRPN - UR;
                                            if (V > 0) PR = PR / V;
                                            process = true;
                                        }
                                        if (!process)
                                        {
                                            if (GRPN >= GRPA)
                                            {
                                                PR = GRPN - UR;
                                                if (V > 0) PR = PR / V;
                                            }
                                            else
                                            {
                                                UR = UR * GRPN / GRPA;
                                                V = 1f - ZR - UR;
                                                PR = GRPN - UR;
                                                if (V > 0) PR = PR / V;
                                            }
                                        }
                                    }
                                } // if (IP != IDSUDRAD)
                            }
                            // FIN DE TRAITEMENT CELLULE 1000
                            RESULCOR[IP, IQ, 1, N1] = ZR;
                            RESULCOR[IP, IQ, 2, N1] = UR;
                            RESULCOR[IP, IQ, 3, N1] = PR;
                            RESULCOR[IP, IQ, 4, N1] = TAU;

                            sortie.Append("N1 = " + N1.ToString());
                            sortie.Append("ZR = " + ZR.ToString());
                            sortie.Append("UR = " + UR.ToString());
                            sortie.Append("PR = " + PR.ToString());
                            sortie.Append(" TAU = " + TAU.ToString("0.0000000"));
                            sortie.Append(" GRPA = " + GRPA.ToString("0.0000000"));
                            sortie.AppendLine(" GRPN = " + GRPN.ToString("0.0000000"));

                        } // for (int N1 = 1; N1 <= 16; N1++)

                    }
                    else
                    {
                        sortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            RESULCOR[IP, IQ, 1, N1] = 1f;
                            RESULCOR[IP, IQ, 2, N1] = 0f;
                            RESULCOR[IP, IQ, 3, N1] = 0f;
                            RESULCOR[IP, IQ, 4, N1] = 0f;
                        }
                    }// if (TREG[1] != 5)
                } // for (int IQ = 1; IQ <= 96; IQ++)
            } // for (int IP = 1; IP <= NBSTA; IP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");
            // FORMAT('CELLULE ', I2, ' Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7, ' GRP=', F9.7, ' GRP''=', F9.7)

            if (File.Exists(pathCnzuptsa)) File.Delete(pathCnzuptsa);
            File.AppendAllText(pathCnzuptsa, sortie.ToString());

            // Ecriture fichier sortie
            FileStream writeStream = new FileStream(pathZuptausaCor, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                    {
                        for (int I = 1; I <= 4; I++)
                        {
                            writeBinary.Write(RESULCOR[IP, IQ, I, N1]);
                        }
                    }
                }
            }
            writeBinary.Close();
            writeStream.Close();
            return RESULCOR;
        }

        private float[,,,] cnzuptdi(ContextPanel contextPanel, int NBIND, int NBSTA, string pathCnzuptdi, string pathCnzuptaudiCor, float[,,,] COUV, byte[,,,] REGRS, float[,,,] RESUL)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALAGE SUR GRP 75000 + ET RECALCUL DES PARAMETRES STATION
            //
            //        DIMANCHE
            //
            int IDSUDRAD = 19;

            float[,,,] RESULCOR = new float[NBSTA + 1, 96 + 1, 4 + 1, contextPanel.NbSeg + 1];
            int[] TREG = new int[contextPanel.NbSeg + 1];

            // BOUCLE STATIONS
            StringBuilder sortie = new StringBuilder();
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                sortie.AppendLine($" Station {IP}");
                Console.WriteLine($"--------STATION {IP} --------");

                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    int NQ = IQ + 76;
                    if (NQ > 96) NQ = NQ - 96;
                    sortie.AppendLine($" QH {IQ}");
                    Console.WriteLine($"Traitement 1/4h {IQ}");

                    // AUCUNE AUDIENCE POUR CE 1 / 4h
                    for (int I = 1; I <= contextPanel.NbSeg; I++)
                    {
                        TREG[I] = REGRS[3, IP, IQ, I];
                    }

                    if (TREG[1] != contextPanel.TRegTest)
                    {
                        float GRPA = 0f;
                        float GRPN = 0f;
                        float ZR = 0f;
                        float UR = 0f;
                        float PR = 0f;
                        float TAU = 0f;
                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            int N2 = contextPanel.SEG1[N1];
                            int N3 = contextPanel.SEG2[N2];
                            int N4 = (contextPanel.Enquete != Enquete.PanelIleDeFrance) ? contextPanel.SEG3[N3] : 0;
                            int IN = TREG[N1] + 1;
                            if (((N1 == 1) || (IN == 1))
                                ||
                               (!((IN == 2 && contextPanel.SEG1[N1] == contextPanel.SEG1[N1 - 1]) ||
                                  (IN == 3 && contextPanel.SEG2[contextPanel.SEG1[N1]] == contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]) ||
                                  (contextPanel.Enquete != Enquete.PanelIleDeFrance && IN == 4 && contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]] == contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1 - 1]]]) ||
                                  (IN == contextPanel.TRegTest))))
                            {

                                // LECTURE DES Z, U, P, TAU ORIGINAUX
                                //   20
                                ZR = RESUL[IP, IQ, 1, N1];
                                UR = RESUL[IP, IQ, 2, N1];
                                PR = RESUL[IP, IQ, 3, N1];
                                TAU = RESUL[IP, IQ, 4, N1];
                                GRPA = (1f - ZR - UR) * PR + UR;

                                if (IP != IDSUDRAD)
                                {
                                    int ISEG = N1;
                                    if (IN == 2) ISEG = contextPanel.SEGBASE[0] + contextPanel.SEG1[N1];
                                    if (IN == 3) ISEG = contextPanel.SEGBASE[1] + contextPanel.SEG2[contextPanel.SEG1[N1]];
                                    if (contextPanel.Enquete != Enquete.PanelIleDeFrance && IN == 4) ISEG = 32 + contextPanel.SEG3[contextPanel.SEG2[contextPanel.SEG1[N1]]];
                                    if (IN == contextPanel.TRegTest) ISEG = contextPanel.IdxTot;

                                    // CALCUL NOUVEAU P
                                    GRPN = COUV[ISEG, 3, IP, NQ];
                                    bool process = false;
                                    if (GRPA != 0d && GRPN != 0d)
                                    {
                                        float V = 1f - ZR - UR;
                                        if (V <= 0d)
                                        {
                                            UR = GRPN;
                                            ZR = 1f - UR;
                                            process = true;
                                        }
                                        else if (GRPN > 1f - ZR)
                                        {
                                            ZR = 1f - GRPN;
                                            V = 1f - ZR - UR;
                                            PR = GRPN - UR;
                                            if (V > 0) PR = PR / V;
                                            process = true;
                                        }
                                        if (!process)
                                        {
                                            if (GRPN >= GRPA)
                                            {
                                                PR = GRPN - UR;
                                                if (V > 0) PR = PR / V;
                                            }
                                            else
                                            {
                                                UR = UR * GRPN / GRPA;
                                                V = 1f - ZR - UR;
                                                PR = GRPN - UR;
                                                if (V > 0) PR = PR / V;
                                            }
                                        }
                                    }
                                } // if (IP != IDSUDRAD)
                            }
                            // FIN DE TRAITEMENT CELLULE 1000
                            RESULCOR[IP, IQ, 1, N1] = ZR;
                            RESULCOR[IP, IQ, 2, N1] = UR;
                            RESULCOR[IP, IQ, 3, N1] = PR;
                            RESULCOR[IP, IQ, 4, N1] = TAU;

                            sortie.Append("N1 = " + N1.ToString());
                            sortie.Append("ZR = " + ZR.ToString());
                            sortie.Append("UR = " + UR.ToString());
                            sortie.Append("PR = " + PR.ToString());
                            sortie.Append(" TAU = " + TAU.ToString("0.0000000"));
                            sortie.Append(" GRPA = " + GRPA.ToString("0.0000000"));
                            sortie.AppendLine(" GRPN = " + GRPN.ToString("0.0000000"));


                        } // for (int N1 = 1; N1 <= 16; N1++)

                    }
                    else
                    {
                        sortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                        {
                            RESULCOR[IP, IQ, 1, N1] = 1f;
                            RESULCOR[IP, IQ, 2, N1] = 0f;
                            RESULCOR[IP, IQ, 3, N1] = 0f;
                            RESULCOR[IP, IQ, 4, N1] = 0f;
                        }
                    }// if (TREG[1] != 5)
                } // for (int IQ = 1; IQ <= 96; IQ++)
            } // for (int IP = 1; IP <= NBSTA; IP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");
            // FORMAT('CELLULE ', I2, ' Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7, ' GRP=', F9.7, ' GRP''=', F9.7)

            if (File.Exists(pathCnzuptdi)) File.Delete(pathCnzuptdi);
            File.AppendAllText(pathCnzuptdi, sortie.ToString());

            // Ecriture fichier sortie
            FileStream writeStream = new FileStream(pathCnzuptaudiCor, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    for (int N1 = 1; N1 <= contextPanel.NbSeg; N1++)
                    {
                        for (int I = 1; I <= 4; I++)
                        {
                            writeBinary.Write(RESULCOR[IP, IQ, I, N1]);
                        }
                    }
                }
            }
            writeBinary.Close();
            writeStream.Close();
            return RESULCOR;
        }

        private bool L1BITFCT(ushort[] ZLEC, int ind)
        {

            int sizeBit = (sizeof(ushort) * 8);
            int index = 1 + ((ind - 1) / sizeBit);
            int pos = NBIT[ind % sizeBit];
            //int pos =ind % sizeBit;
            return BTEST(ZLEC[index], pos);
        }

        private bool BTEST(ushort value, int pos)
        {
            return (value & (ushort)(1 << pos)) != 0;
        }

        private static double[] RES = new double[4];
        private static double DELTATO = 0d;

        // SUBROUTINE MINIMISE
        // optimisation de la courbe theorique de montee en audience
        // suivant la beta binomiale
        private void MINIMISE(double GRP, double[] ZC, ref double PR, ref double UR, ref double ZR, out double TAU, double ZA, double UA, float NB, bool qhps = false)
        {
            double DZ, DZ0, DT0, Z, Z1, Z2, DELTA0, DELTZ0, DELTZ1, DELTZ2, DELTA00, DELTZ = 0d;
            RES = new double[5] { 0, ZA, UA, 0d, 0d };
            double V0 = 1d - ZA - UR;
            if (V0 > 0.00005)
            {
                DELTA0 = 0f;
                for (int I = 1; I <= NB; I++)
                {
                    DELTA0 = DELTA0 + 2f * I * I * ZC[I] * ZC[I];
                }
                DELTA00 = DELTA0;
                DELTATO = DELTA0;
                DELTZ0 = DELTA0;
                DELTZ1 = DELTA0;
                DELTZ2 = DELTA0;
                DZ0 = DT0 = DZ = 0.1d;

                Z = ZA + (1f - ZA) / 2;
                //if (!qhps || Z >= ZR)  // NE PAS VIRER => petite variation remettre quand sav1qhpa marchera
                //{
                if ((1f - Z) < DZ) DZ = 1d - Z - 0.00001d;
                Z1 = Z;
                Z2 = ZR;
                double U0 = UR;
                //5 
                
                for (; ; )
                {
                    bool sortie = false;
                    float ITZ = 1;
                    if (Z >= ZR)
                    {
                        if (DZ > (Z1 - Z2)) DZ = Z1 - Z2;
                        // VARIATION DES JAMAIS
                        //10 
                        for (; ; ITZ++)
                        {

                            MINIMU(Z, GRP, ZC, ZA, UA, U0, NB, DELTA00, ref DELTZ);

                            if (DELTZ <= DELTZ0)
                            {
                                if (qhps && (DELTZ == DELTZ0 && DELTZ0 == DELTZ1 && DELTZ1 == DELTZ2))
                                {
                                    ZR = RES[1];
                                    UR = RES[2];
                                    PR = RES[3];
                                    TAU = RES[4];
                                    return;
                                }

                                // LA DISTANCE A DIMINUE
                                DELTZ2 = DELTZ1;
                                DELTZ1 = DELTZ0;
                                DELTZ0 = DELTZ;
                                //if (DZ <= 0.0001d)
                                //{
                                //    sortie = true;
                                //    break; // GO TO 260
                                //}
                                if (Z > Z2)
                                {
                                    Z -= DZ;
                                    continue; // GOTO 10
                                }
                                if (DZ <= 0.0001d)
                                {
                                    ZR = RES[1];
                                    UR = RES[2];
                                    PR = RES[3];
                                    TAU = RES[4];
                                    return;
                                }
                                Z2 = Z;
                                Z += DZ * 2;
                                DZ /= 10d;
                            }
                            else
                            {
                                // LA DISTANCE A AUGMENTE
                                if (ITZ > 2)
                                {
                                    if (DZ <= 0.0001d)
                                    {
                                        ZR = RES[1];
                                        UR = RES[2];
                                        PR = RES[3];
                                        TAU = RES[4];
                                        return;
                                    }
                                    Z2 = Z;
                                    Z += 2d * DZ;
                                    DZ /= 10d;
                                    DELTZ0 = DELTZ2;
                                    sortie = true;
                                    break; // GOTO 5
                                }
                                if (DZ <= 0.0001d)
                                {
                                    ZR = RES[1];
                                    UR = RES[2];
                                    PR = RES[3];
                                    TAU = RES[4];
                                    return;
                                }
                                Z2 = Z;
                                Z = Z1;
                                DZ /= 10d;
                                DELTZ0 = DELTZ1;
                                sortie = true;
                                break; // GOTO 5
                            }
                        } // for (; ; )  du 10
                        //if (sortie) break; // ==> goto 5
                    }// if (Z >= ZR)
                    else
                    {
                        ZR = RES[1];
                        UR = RES[2];
                        PR = RES[3];
                        TAU = RES[4];
                        return;
                    }
                } // for (; ; )  du 5
            } // if (V0 > 0.00005)

            ZR = ZA;
            TAU = 0f;
        }

        // SUBROUTINE MINITAU
        // CALCUL DE LA VALEUR OPTIMALE DE TAU
        private void MINITAU(double[] ZC, double[] X, double ZA, float NB, double U0, double Q, double Z, double U, double P, double V, out double DELTT, ref double DELTA00)
        {
            double[] Y = new double[15 + 1];
            double T, T1, T2, DT;
            double DMOINS1 = 0, DMOINS2 = 0, DELTA0;

            // INCREMENTS INITIAUX
            DELTT = 0;
            DELTA0 = DELTA00;
            DT = 0.1d;
            T1 = 0d;
            T2 = 2d;

            // VARIATION DE LA DISPERSION
            int ITE = 0;
            T = T1;
            for (; ; )
            {
                ITE = ITE + 1;
                // EVALUATION DE LA DISTANCE
                for (int N = 2; N <= NB; N++)
                {
                    double A = (N - 1) * T;
                    X[N] = X[N - 1] * (Q + A) / (1 + A);
                }
                for (int K = 1; K <= NB; K++)
                {
                    Y[K] = (1d - V * X[K] - Z);
                }
                double DELTA = 0d;
                for (int K = 1; K <= NB; K++)
                {
                    DELTA = DELTA + K * (float)Math.Pow(ZC[K] * 0.01d - Y[K], 2);
                }
                DELTA = DELTA + 0.1d * (float)Math.Pow(ZA - Z, 2) / ZA;

                if (U0 > 0) DELTA = DELTA + 0.1d * (float)Math.Pow(U0 - U, 2) / U0;

                if (DELTA <= DELTA0)
                {
                    // LA DISTANCE A DIMINUE
                    DMOINS2 = DMOINS1;
                    DMOINS1 = DELTA0;
                    DELTA0 = DELTA;

                    if (DELTA < DELTATO)
                    {
                        DELTATO = DELTA;
                        RES[4] = T;
                        RES[3] = P;
                        RES[2] = U;
                        RES[1] = Z;
                    }

                    if (T < T2)
                    {
                        T = T + DT;
                        continue;
                    }

                    if (DT != 0.1d) break;
                }
                else
                {
                    // LA DISTANCE A AUGMENTE
                    if (ITE > 2 && DT > 0.0001d)
                    {
                        DELTA0 = DMOINS2;
                        T1 = T - 2d * DT;
                        T2 = T;
                        DT = DT / 10d;
                        ITE = 0;
                        T = T1;
                        continue;
                    }
                    if (ITE <= 2 && DT > 0.0001d)
                    {
                        T2 = T;
                        DT = DT / 10d;
                        ITE = 0;
                        T = T1;
                        continue;
                    }

                    DELTT = DELTA0;
                    break;
                }

                T2 = T2 * 2d;
                T1 = T1 * 2d;

                if (T2 < 16d)
                {
                    ITE = 0;
                    T = T1;
                }
                else
                    break;
            }
            //    Console.WriteLine($" U= {U}  Z= {Z}  T= {T}  P= {P} ");
        }


        // SUBROUTINE MINIMU
        // CALCUL DU U OPTIMAL
        // EN FONCTION DE Z
        private void MINIMU(double Z, double GRP, double[] ZC, double ZA, double UA, double U0, float NB, double DELTA00, ref double DELTZ)
        {
            double[] X = new double[15 + 1];

            double P, U, V, Q, U1, U2, DELTU, DELTU0, DELTU1, DELTU2;

            DELTU0 = DELTA00;
            DELTU1 = DELTU0;
            DELTU2 = DELTU0;
            DELTU = 0;

            // INCREMENTS INITIAUX
            double DU = 0.01d;
            U1 = 0f;
            U = UA;
            U2 = 1d - Z;
            U = U1;
            if (U0 == 0) U = 0d;

            int ITU = 0;
            for (; ; )
            {
                // VARIATION DES TOUJOURS       
                V = 1d - Z - U;
                if (V > 0d)
                {
                    P = (GRP - U) / V;
                    ITU++;
                    Q = 1d - P;
                    X[1] = Q;

                    MINITAU(ZC, X, ZA, NB, U0, Q, Z, U, P, V, out DELTU, ref DELTA00);
                    if (U0 == 0)
                    {
                        DELTZ = DELTU;
                        return;
                    }
                    if (DELTU <= DELTU0)
                    {
                        // LA DISTANCE A DIMINUE
                        DELTU2 = DELTU1;
                        DELTU1 = DELTU0;
                        DELTU0 = DELTU;
                        if (U < U2 && U < (1d - Z))
                        {
                            U = U + DU;
                            if (U >= (1d - Z))
                            {
                                DU = U - 1d - Z;
                                U = 1d - Z - 0.00001d;
                            }
                            continue;
                        }
                        if (DU <= 0.00001d)
                        {
                            DELTZ = DELTU0;
                            return;
                        }
                        U2 = U;
                        if (U2 > (1d - Z)) U2 = 1d - Z;
                        U1 = U - DU;
                        if (U1 < 0) U1 = 0;
                        U = U1;
                        DU = DU / 10d;
                        continue;
                    }
                    else
                    {
                        // LA DISTANCE A AUGMENTE
                        if (DU <= 0.00001d)
                        {
                            DELTZ = DELTU0;
                            return;
                        }
                        if (ITU >= 2) U1 = U - 2 * DU;
                        if (ITU < 2)
                        {
                            U1 = U - DU;
                            if (U >= (1d - Z)) DU = U - 1d - Z;
                            if (U >= (1d - Z)) U = 1d - Z - 0.00001d;
                        }
                        if (U1 < 0) U1 = 0;
                        U2 = U;
                        U = U1;
                        DU = DU / 10d;

                        if (ITU > 2) DELTU0 = DELTU2;
                        if (ITU <= 2) DELTU0 = DELTU1;
                        ITU = 0;
                        continue;
                    }
                } // if (V > 0)
                if (DU > 0.00001d)
                {
                    DU = DU / 10d;
                    U = U1;
                    ITU = 0;
                    continue;
                }
                else
                    return;
            } // for (; ; )
        }
    }

    public struct Fushab09Indiv
    {
        public byte[] AVANT;
        public byte[,] KHAB;
        public byte[,] KHSA;
        public byte[,] KHDI;
        public byte[] APRES;
        public byte[] CHARIOT;
    }

    #region Structures
    public struct Regr5jp2
    {
        public int[][,] KHI3;
        public int[] IG;
        public Regr5jp2(int LENENR)
        {
            KHI3 = new int[3][,];
            for (int i = 1; i <= 3; i++)
                KHI3[i] = new int[LENENR, 5];
            IG = new int[3];
        }
    }

    public struct ContextPanel
    {
        public Enquete Enquete;
        public int[] SEG1;
        public int[] SEG2;
        public int[] SEG3;
        public int[] SEGBASE; // 16,26,32  ou  5,8 idf
        public int IdxTot;    // 37 ou 11 idf
        public int NbSeg;     // 16 ou 5 idf
        public int TRegTest;  // 5 ou 4 idf
        public int[,] NIVO;
        public int Year;
        public int SIGN_LINE_LEN_BEFORE_HAB;
        public int SIGN_LINE_LEN_AFTER_HAB;
        public int NB_STA_ALL_HAB;

        public List<int> Age35a59;
        public List<int> Age60Plus;

        public List<int> Age20a24;
        public List<int> Age25a34;
        public List<int> Age35a49;
        public List<int> Age50a64;
        public List<int> Age65Plus;

        public int IND_CSP;
        public int IND_AGE;
        public int IND_SEX;
        public int IND_REG;

        public int COL_AGE3;
        public int COL_RUDA;
        public int COL_PIAB;
        public int COL_CSCI;
        public int COL_SEX;
        public int COL_AGE11;
        public int COL_HAB;
        public int COL_MENA;
        public int COL_RDA;
        public int COL_CSCC;
        public int COL_NIEL;
        public int COL_PG22;
        public int COL_ENFA;
        public int COL_ENF1;
        public int COL_ENF2;
        public int COL_ENF3;
        public int COL_ENF4;
        public int COL_NPER;
        public int COL_APRES;
        public int COL_CELL;

        public int COL_TAB_CSCI;
        public int COL_TAB_SEX;
        public int COL_TAB_AGE11;
        public int COL_TAB_HAB;
        public int COL_TAB_MENA;
        public int COL_TAB_RDA;
        public int COL_TAB_CSCC;
        public int COL_TAB_NIEL;
        public int COL_TAB_PG22;
        public int COL_TAB_ENFA;
        public int COL_TAB_ENF1;
        public int COL_TAB_ENF2;
        public int COL_TAB_ENF3;
        public int COL_TAB_ENF4;
        public int COL_TAB_NPER;
        public int COL_TAB_APRES;
        public int COL_TAB_CELL;
        public int COL_TAB_AGE3;
        public int COL_TAB_RUDA;
        public int COL_TAB_PIAB;
    }

    public struct VsorPoid
    {
        public ushort[,] VSor;
        public int[] Poid;
        public VsorPoid(int NBSTA)
        {
            VSor = new ushort[6 + 1, NBSTA];
            Poid = new int[96 + 1];
        }
    }
    #endregion Structures
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace ARProbaProcessing
{
    public partial class AffinageProcess
    {
        private int[] NOTE = new int[25 + 1] { 999999, 0, 12, 6, 4, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        private int[] NBIT = new int[] { 999999, 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
        private int[] SEG1 = new int[] { 999999, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 9, 10, 10 };
        private int[] SEG2 = new int[] { 999999, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6 };
        private int[] SEG3 = new int[] { 999999, 1, 2, 2, 3, 3, 4 };

        public void Run()
        {
            int NbStation = 0;
            int NbGRPModulation = 0;
            int NbGRPStation = 0;
            int[] NotorieteStation = null;
            string PathGRPWave = @"C:\AffinageART\France\Source\SFR04\U120";
            List<int> lstPoids;
            List<int> lstAges;
            List<int> lstFiltreIDF;
            int nbJour = 23;

            lecpanel(0, "", 0, 0, 0, 0, 0, 0, 0, out lstPoids, out lstAges, out lstFiltreIDF);

            #region entrées Fushab09
            int SIGN_LINE_LEN_BEFORE_HAB = 0;
            int NB_STA_ALL_HAB = 0;
            int[] TABRH = null;
            #endregion

            #region entrées segpanel
            string pathSIGJFC_BDE = @"c:\Affinage\Panel_National\PanFra20\Input\F04\sig20jfc2.bde";
            string pathSortie1 = @"C:\AffinageART\France\Source\SFR04\OUTPUT\SORTIE1.TXT";
            #endregion entrées segpanel

            #region entrées ecrpan1
            string pathF04 = @"c:\Affinage\Panel_National\PanFra20\Input\F04\";
            int nbStationTotal = NbStation;
            int year = 2020;
            int[] ITS = new int[] { 999999, 16, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, -35, 36, 37, 39, 47, 50, 53, 54, 55, 56, 59, 60, 61, 64, 1 };
            #endregion entrées ecrpan1

            #region entrées ecrsegpa
            string pathSig = @"C:\AffinageART\France\Source\SFR04\U120\";
            int COL_PIAB = 9;
            int COL_CSCI = 18;
            int COL_SEX = 14;
            int COL_AGE = 24;
            int COL_RUDA = 20;
            int SIGN_LINE_LEN_FULL = 694;
            #endregion entrées ecrsegpa

            #region entrees Calcregr
            int NB_STA_HAB_NOTO = NbStation;
            #endregion entrees Calcregr

            #region entrées cont75br
            int popLV = 54439040;
            int popS = 54438920;
            int popD = 54439190;
            string pathSortie9 = @"C:\AffinageART\France\Source\SFR04\OUTPUT\SORTIE9.TXT";
            #endregion entrées cont75br

            #region entrées attribp2
            string pathCnzuptse = @"C:\AffinageART\France\Source\SFR04\OUTPUT\SORTIESE.COR";
            string pathCnzuptsa = @"C:\AffinageART\France\Source\SFR04\OUTPUT\SORTIESA.COR";
            string pathCnzuptdi = @"C:\AffinageART\France\Source\SFR04\OUTPUT\SORTIEDI.COR";
            #endregion entrées cnzuptse..

            #region entrées attribp2
            string pathAttribp2 = @"C:\AffinageART\France\Source\SFR04\OUTPUT\SORTIE10.TXT";
            #endregion entrées attribp2

            #region entrées Transp08
            int NB_STA_IDF = 14;
            string pathTransp08 = @"C:\AffinageART\France\Source\SFR04\OUTPUT\2020.SUP";
            string pathYearNat = @"C:\AffinageART\France\Source\SFR04\OUTPUT\PANRA120.NAT";
            string pathYearIdf = @"C:\AffinageART\France\Source\SFR04\OUTPUT\PANRA120.IDF";
            string pathYearSup = @"C:\AffinageART\France\Source\SFR04\OUTPUT\PANRA120.SUP";
            // ISTA : IDF & No Sud Radio ...
            int[] ISTA = new int[] { 999999, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 };
            #endregion entrées Transp08

            #region entrées crecib08
            int COL_MENA = 15;
            string pathPan20Cib = @"C:\AffinageART\France\Source\SFR04\OUTPUT\PAN20CIB";
            #endregion entrées crecib08

            #region entrées penetr
            string pathPenetr = @"C:\AffinageART\France\Source\SFR04\OUTPUT\penetr";
            int NB_STA_HAB_NOTO_TOTAL = 30;
            List<string> strStations = new List<string>();
            strStations.Add("x!x");
            strStations.Add("                                Les Indés Radios ");
            strStations.Add("                                        Chérie FM");
            strStations.Add("                                         Europe 1");
            strStations.Add("                                     Virgin Radio");
            strStations.Add("                                   France Culture");
            strStations.Add("                                      France Info");
            strStations.Add("                                     France Inter");
            strStations.Add("                                   France Musique");
            strStations.Add("                                        Fun Radio");
            strStations.Add("                                          M Radio");
            strStations.Add("                                        Nostalgie");
            strStations.Add("                                              NRJ");
            strStations.Add("                                      France Bleu");
            strStations.Add("                                              RFM");
            strStations.Add("                                              RMC");
            strStations.Add("                                              RTL");
            strStations.Add("                                             RTL2");
            strStations.Add("                                          Skyrock");
            strStations.Add("                                 Rire et Chansons");
            strStations.Add("                     NRJ Global Proximité Premium");
            strStations.Add("                                 Nova and Friends");
            strStations.Add("                     Lagardère Publicité News IDF");
            strStations.Add("                                       Radio Nova");
            strStations.Add("                                            Lip !");
            strStations.Add("                                    Paris – IDF +");
            strStations.Add("                                  Radio Classique");
            strStations.Add("                               Les Indés Capitale");
            strStations.Add("                                   TF1 Pub Radios");
            strStations.Add("                                         TSF JAZZ");
            strStations.Add("                                      Total Radio");

            int population = 54439040;
            #endregion entrées penetr

            #region entrées Asymp
            string pathAS5H5H = @"C:\AffinageART\France\Source\SFR04\OUTPUT\AS5H5H";
            string headerAS5H5H = "             PANEL MEDIAMETRIE RADIO 2008" +
                Environment.NewLine +
                " PENETRATIONS CUMULEES MAXIMALES 5H-29H LUNDI-DIMANCHE " +
                Environment.NewLine + Environment.NewLine + Environment.NewLine +
                " STATIONS/AGREGATS      ENSEMBLE  HOMMES FEMMES     13-34 ANS";
            // TODO: différence entre le fichier template quer j'ai avec la sortie du fichier donné par Fred
            // Template : ...."STATIONS/AGREGATS      ENSEMBLE  MENAGERES    13-28'-/ 24X,'13 ANS +   - 50 ANS'
            #endregion entrées Asymp


            int NBINDIV = segpanel(COL_PIAB, COL_CSCI, COL_SEX, COL_AGE, COL_RUDA, pathSIGJFC_BDE,  pathSortie1);

            VsorPoid[][] JN = ecrpan1j(pathF04, NbStation, nbStationTotal, ITS, year);   // [Jour 1..23][Individus 1..N] = {VOSR[,]?, Poid[]}

            VsorPoid[][][] JNByWeek = regr5jp2(JN); // [Semaine 1..3][Jour de semaine Lundi à vendredi 1..5][indiv]

            List<Fushab09Indiv> fushab09Indivs = Fushab09(NbStation, SIGN_LINE_LEN_BEFORE_HAB, NB_STA_ALL_HAB, TABRH);

            int[,,] NINI_IND_QH_W = chab1qhp(NbStation, fushab09Indivs); // Habitude [INDIV, QH, LV/Sa/Di];

            int[,] NINI_IND_STA = crenonin(nbJour, NbStation, fushab09Indivs, JN); // [INDIV, STATIONS] 

            ecrsegpa(pathSig, COL_PIAB, COL_CSCI, COL_SEX, COL_AGE, COL_RUDA, NbStation, NBINDIV, SIGN_LINE_LEN_FULL,
                     out int[,] POIDSEGM, out int IPOP);

            int[,,,] cellules = calcregr(NBINDIV, NB_STA_HAB_NOTO, NINI_IND_STA, POIDSEGM, JN); // int[LV/Sa/Di, STATIONS, QH, CELL];

            int[,,,] regrs = calcnivo(NBINDIV, NbStation, cellules); // [LV/Sa/Di, STATIONS, QH, CELL]

            int[,,,] audiences = caud1qhp(NBINDIV, NB_STA_HAB_NOTO, JN, POIDSEGM); // audiences[STATIONS, INdiv, QH, 1..3]

            double[] noteIndiv = caudtotp(NBINDIV, NB_STA_HAB_NOTO, JN, POIDSEGM, fushab09Indivs);

            double[,,,] ZUPTAUSE = sav1qhpa(NBINDIV, NB_STA_HAB_NOTO, regrs, POIDSEGM, fushab09Indivs, JNByWeek, JN); // [STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL];

            double[,,,] ZUPTAUSA = sav1qhps(NBINDIV, NB_STA_HAB_NOTO, regrs, POIDSEGM, fushab09Indivs, JNByWeek, JN); // [STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL];

            double[,,,] ZUPTAUDI = sav1qhpd(NBINDIV, NB_STA_HAB_NOTO, regrs, POIDSEGM, fushab09Indivs, JNByWeek, JN); // [STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL];

            double[,,,] Couverture = cgrp75br(PathGRPWave, NbStation, NbGRPModulation, NbGRPStation, NotorieteStation);  // [LV/Sa/Di, QH, 4 + 1, CELL];

            cont75br(NBINDIV, NB_STA_HAB_NOTO, popLV, popS, popD, Couverture, pathSortie9);

            double[,,,] ZUPTAUSECOR = cnzuptse(NBINDIV, NB_STA_HAB_NOTO, pathCnzuptse, Couverture, regrs, ZUPTAUSE);  // double[STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL];

            double[,,,] ZUPTAUSACOR = cnzuptsa(NBINDIV, NB_STA_HAB_NOTO, pathCnzuptsa, Couverture, regrs, ZUPTAUSE);

            double[,,,] ZUPTAUDICOR = cnzuptdi(NBINDIV, NB_STA_HAB_NOTO, pathCnzuptdi, Couverture, regrs, ZUPTAUSE);

            int[,,,] PROBAS = attribp2(NBINDIV, NB_STA_HAB_NOTO, regrs, POIDSEGM, NINI_IND_STA, noteIndiv, audiences, NINI_IND_QH_W,
                ZUPTAUSECOR, ZUPTAUSACOR, ZUPTAUDICOR, pathAttribp2); // [STATIONS, LV/Sa/Di, QH, INDIVS]

            BSupport BSUP = transp08(NBINDIV, NB_STA_HAB_NOTO, NB_STA_IDF, ISTA, POIDSEGM, lstFiltreIDF, lstPoids, PROBAS, pathTransp08, pathYearNat, pathYearIdf, pathYearSup);

            int[,] PANCIB = crecib08(NBINDIV, fushab09Indivs, COL_AGE, COL_MENA, pathPan20Cib); // [3 + 1, NIND + 1]

            penetr(NBINDIV, NB_STA_HAB_NOTO_TOTAL, JN, lstPoids, pathPenetr, population, strStations);

            asympt(NBINDIV, NB_STA_HAB_NOTO, NB_STA_HAB_NOTO_TOTAL, BSUP, PANCIB, pathAS5H5H, headerAS5H5H, strStations);
        }

        private void lecpanel(int SIGN_LINE_LEN_FULL, string Path_SIGJFC_BDE, int COL_AGE3, int COL_RUDA, int COL_PIAB_0, int COL_PIAB_1, int COL_PIAB_2, int COL_PIAB_3, int COL_PIAB_4, out List<int> lstPoids, out List<int> lstAges, out List<int> lstFiltreIDF)
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
            int[] KHI2 = new int[SIGN_LINE_LEN_FULL + 1];

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


            //OPEN(14, FILE = '#OUTPUT#POIDS',
            //    -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')

            lstPoids = new List<int>();


            //OPEN(15, FILE = '#OUTPUT#AGES',
            //    -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')

            lstAges = new List<int>();


            //OPEN(16, FILE = '#OUTPUT#FILTREIF',
            //    -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')

            lstFiltreIDF = new List<int>();


            //                              BOUCLE INDIVIDUS

            FileStream fs = File.Open(Path_SIGJFC_BDE, FileMode.Open);
            fs.Seek(0, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fs);

            while (fs.Position != fs.Length)
            {

                //30

                for (int i = 1; i < KHI2.Length; i++)
                    KHI2[i] = br.ReadUInt16();

                IAGE = 1;

                if ((KHI2[COL_AGE3] - 48) == 1)
                    IAGE = 2;

                // Filtre REGION PARISIENNE(RÃ©gion Uda)

                ICL = 0;

                if ((KHI2[COL_RUDA] - 48) == 1)
                {
                    ICL = 1;
                    COMPTIDF = COMPTIDF + 1;
                }



                // CALCUL DU POIDS(Colonnes 9 Ã  13 inclues)

                IPERS = 10000 * (KHI2[COL_PIAB_0] - 48) + 1000 * (KHI2[COL_PIAB_1] - 48) + 100 * (KHI2[COL_PIAB_2] - 48) + 10 * (KHI2[COL_PIAB_3] - 48) + (KHI2[COL_PIAB_4] - 48);

                if (IPERS <= 0) continue;


                //if (IPERS > 3276) PRINT * , ' IPERS= ', IPERS

                // On comptabilise les individus

                IG = IG + 1;


                IPOP = IPOP + IPERS * 10;


                //WRITE(14) IPERS
                lstPoids.Add(IPERS);


                //WRITE(15) IAGE
                lstAges.Add(IAGE);


                //WRITE(16) ICL
                lstFiltreIDF.Add(ICL);


            }
            //                                                FIN DE FICHIER

            //120 WRITE(6, 1) IG, IPOP, COMPTIDF
            Console.WriteLine("NbIndiv : " + IG + " Population : " + IPOP + " NbIndivIDF : " + COMPTIDF);


            //1 FORMAT('1   NBGUS:', I6, ' POPULATION:', I9, ' Individus IDF:', I12)


        }

        private int segpanel(int COL_PIAB, int COL_CSCI, int COL_SEX, int COL_AGE, int COL_RUDA, string pathSIGJFC_BDE, string pathSortie1)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CONTROLE DES VOLUMES DES SEGMENTS
            //
            // Longueur d'une ligne: 599 + 2 car de fin
            int[] SEGM = new int[16];
            int ICSP, IREG, ISEX, IAGE, ISEG, IPERS, AGE;

            // Nous avons besoin de la variable AGE pour le calcul de l'age qui est sur deux colonnes

            int IG = 0;
            int IPOP = 0;
            IEnumerable<string> KHI2 = File.ReadLines(pathSIGJFC_BDE);
            foreach (string strIndiv in KHI2)
            {
                // CALCUL DU POIDS(Colonnes 9 à 13 inclues)
                IPERS = int.Parse(strIndiv.Substring(COL_PIAB, 5));

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
                if ((strIndiv[COL_CSCI] == '1') || (int.Parse(strIndiv[COL_CSCI].ToString()) > 4)) ICSP = 2;
                if (int.Parse(strIndiv[COL_CSCI].ToString()) > 6) ICSP = 3;

                //  --------------------
                //   Sexe de l'interviewé
                //  --------------------
                //   Le sexe de l'interviewé est codé sur une colonne (colonne 14)
                //   ISEX = 1 correspond aux Hommes
                //   ISEX = 2 correspond aux Femmes

                ISEX = int.Parse(strIndiv[COL_SEX].ToString());

                //  ----------------- -
                //   Age de l'interviewé
                //  ------------------ -
                //  L'Age de l'interviewé est codé sur deux colonnes(colonne 24 et 25)
                //  IAGE = 1 correspond aux 13 - 34 ans
                //  IAGE = 2 correspond aux 35 - 59 ans
                //  IAGE = 3 correspond aux 60 ans et +
                AGE = int.Parse(strIndiv.Substring(COL_AGE, 2));
                IAGE = 1;
                if (AGE > 4) IAGE = 2;
                if (AGE > 9) IAGE = 3;

                //  --------------------------
                //   Région UDA de l'interviewé
                //  --------------------------
                //   La région UDA de l'interviewé est codé sur une colonne (colonne 20)
                //   IREG = 1 correspond aux 6 UDA Nord
                //   IREG = 2 correspond aux 3 UDA Sud

                IREG = 1;
                if (int.Parse(strIndiv[COL_RUDA].ToString()) > 6) IREG = 2;

                //C-------------------------------------------------------------------------- -
                //  C On fonction des variables précédemment calculées, on détermine les segments
                //  C-------------------------------------------------------------------------- -
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

            } // foreach (string strIndiv in KHI2)

            if (File.Exists(pathSortie1)) File.Delete(pathSortie1);
            StreamWriter swSortie1 = new StreamWriter(File.Create(pathSortie1));
            swSortie1.WriteLine($"NBGUS: {IG}   POPULATION: {IPOP}");

            for (int I = 1; I <= 16; I++)
            {
                double PEN = SEGM[I];
                PEN = PEN * 100d / IG;
                swSortie1.WriteLine($"CLASSE: {I}   VOLUME : {SEGM[I]}   POURCENTAGE: {PEN} ");
            }

            return IG;
        }

        private VsorPoid[][] ecrpan1j(string pathF04, int NBSTA, int NB_STA_TOTAL, int[] ITS, int year)
        {
            VsorPoid[][] resultJn = new VsorPoid[23 + 1][];
            for (int sor = 1; sor <= 24; sor++)
                resultJn[sor] = new VsorPoid[NBSTA];

            // PANEL RADIO 08 MEDIAMETRIE(Nouveau format)
            // CREATION DU POIDS DES 1 / 4H POUR 1 JOUR

            //  Attention: ne pas oublier de changer NBSTA avec le nombre de stations utiliser dans hab et noto
            //PARAMETER(NBSTA =#NB_STA_HAB_NOTO_TOTAL#)

            int[] KHI2 = new int[3 + 1];

            // Attention: ne pas oublier de changer VRAD avec le nombre de stations totals

            int[] KDEB = new int[130 + 1];
            int[,] VRAD = new int[6 + 1, NB_STA_TOTAL + 1];
            int[] ITAP = new int[96 + 1];

            // INITIALISATIONS
            int IG;

            // OUVERTURE FICHIERS
            for (int IJ = 1; IJ <= 23; IJ++)
            {
                IG = 0;

                string pathJN = Path.Combine(pathF04, "JN" + IJ.ToString("00") + "NI" + (year % 100).ToString("00") + ".F04");
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

                    for (int i = 1; i <= 6; i++)
                        for (int j = 1; j <= NB_STA_TOTAL; j++)
                            VRAD[i, j] = br.ReadInt16();

                    if (KHI2[1] == 1)
                    {
                        IG = IG + 1;

                        int[,] VSOR = new int[6 + 1, NBSTA + 1];
                        for (int IS = 1; IS <= NBSTA; IS++)
                        {
                            for (int I = 1; I <= 6; I++)
                            {
                                VSOR[I, IS] = VRAD[I, ITS[IS]];
                            }
                        }

                        for (int IS = 1; IS <= NBSTA - 2; IS++)
                        {
                            int IC = ITS[IS];
                            for (int I = 1; I <= 96; I++)
                            {
                                int[] bits = new int[7];
                                for (int b = 1; b <= 6; b++)
                                    bits[b] = VRAD[b, IC];
                                if (L1BITFCT(bits, I)) ITAP[I] = ITAP[I] + 1;
                            }
                        }

                        int[] POID = new int[96 + 1];
                        for (int I = 1; I <= 96; I++)
                        {
                            POID[I] = NOTE[ITAP[I] + 1];
                        }

                        resultJn[IJ][IG] = new VsorPoid(NBSTA) { Poid = POID, VSor = VSOR };
                    }
                }

                // FIN DE FICHIER
                Console.WriteLine(IJ.ToString() + " " + IG.ToString());
            }

            // ???? Console.WriteLine($"JOUR : {I2}   NBGUS : {I6});
            return resultJn;
        }


        private VsorPoid[][][] regr5jp2(VsorPoid[][] JN)
        {
            VsorPoid[][][] JNByWeek = new VsorPoid[3 + 1][][];   // [Semaine 1..3][Jour de semaine Lundi à vendredi 1..5][indiv]

            int nbIndiv = JN[0].Length;
            for (int week = 1; week <= 3; week++)
            {
                JNByWeek[week] = new VsorPoid[5 + 1][];
                for (int j = 1; j <= 5; j++)
                {
                    JNByWeek[week][j] = new VsorPoid[nbIndiv];

                    for (int indiv = 1; indiv <= nbIndiv; indiv++)
                    {
                        int noJour = 2 + (week - 1) * 7 + j;
                        JNByWeek[week][j][indiv] = JN[noJour][indiv];                     // JN[Jour 1..23][Individus 1..N] = {VOSR[,]?, Poid[]}
                    }
                }
            }

            return JNByWeek;
        }

        public List<Fushab09Indiv> Fushab09(
            int NBSTA,
            int SIGN_LINE_LEN_BEFORE_HAB,
            int NB_STA_ALL_HAB,
            int[] TABRH)
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
            int[,] KHA2 = new int[9 + 1, NBSTA + 1];
            int[,] KHS2 = new int[9 + 1, NBSTA + 1];
            int[,] KHD2 = new int[9 + 1, NBSTA + 1];

            // => EN PARAMETRE
            // INTEGER * 2 TABRH(NBSTA)
            // C Attention pour les stations avec Notoriété il faut metre les données total radio(#TOTAL_RADIO_INDEX#)
            // int[] TABRH = new int[NBSTA + 1]; 
            // int[] TABRH /#HAB_STA_LIST_ID_NOTO_SET_TO_TOTAL_RADIO#/

            // INTEGER*1 int KHAB(9,NB_STA_ALL_HAB),KHSA(9,#NB_STA_ALL_HAB#),KHDI(9,#NB_STA_ALL_HAB#),APRES[#SIGN_LINE_LEN_AFTER_HAB#), CHARIOT(2)
            int[] AVANT = new int[SIGN_LINE_LEN_BEFORE_HAB + 1];
            int[,] KHAB = new int[9 + 1, NB_STA_ALL_HAB + 1];
            int[,] KHSA = new int[9 + 1, NB_STA_ALL_HAB + 1];
            int[,] KHDI = new int[9 + 1, NB_STA_ALL_HAB + 1];
            int[] APRES = new int[SIGN_LINE_LEN_BEFORE_HAB + 1];
            int[] CHARIOT = new int[2];

            // INITIALISATIONS
            int G = 0;

            // OUVERTURE FICHIERS
            Console.WriteLine("NBSTA=" + NBSTA);
            Console.WriteLine("SIGN_LINE_LEN_BEFORE_HAB=" + SIGN_LINE_LEN_BEFORE_HAB);
            Console.WriteLine("NB_STA_ALL_HAB=" + NB_STA_ALL_HAB);

            //
            //                              OUVERTURE FICHIER
            //
            // OPEN(13, FILE = '#SIGJFC_BDE#', -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')
            FileStream fs = File.Open(@"C:\AffinageART\France\Source\SFR04\U120\sig20jfc.bde", FileMode.Open);
            fs.Seek(0, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fs);

            int IG = 0;
            List<Fushab09Indiv> fushab09Indivs = new List<Fushab09Indiv>();

            // BOUCLE INDIVIDUS
            while (fs.Position != fs.Length)
            {
                // 30 READ(13, END = 120) AVANT,((KHAB(I, J), I = 1, 9), J = 1,#NB_STA_ALL_HAB#),((KHSA(I,J),I=1,9),J=1,#NB_STA_ALL_HAB#),
                // -((KHDI(I,J),I=1,9),J=1,#NB_STA_ALL_HAB#),APRES,CHARIOT

                for (int i = 1; i <= SIGN_LINE_LEN_BEFORE_HAB; i++)
                    AVANT[i] = br.ReadByte();

                for (int i = 1; i <= 9; i++)
                    for (int j = 1; j <= NB_STA_ALL_HAB; j++)
                        KHAB[i, j] = br.ReadByte();

                for (int i = 1; i <= 9; i++)
                    for (int j = 1; j <= NB_STA_ALL_HAB; j++)
                        KHSA[i, j] = br.ReadByte();

                for (int i = 1; i <= 9; i++)
                    for (int j = 1; j <= NB_STA_ALL_HAB; j++)
                        KHDI[i, j] = br.ReadByte();

                for (int i = 1; i <= SIGN_LINE_LEN_BEFORE_HAB; i++)
                    APRES[i] = br.ReadByte();

                for (int i = 1; i <= 2; i++)
                    CHARIOT[i] = br.ReadByte();

                IG = IG + 1;

                // BOUCLE STATIONS

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
                fushab09Indivs.Add(new Fushab09Indiv(SIGN_LINE_LEN_BEFORE_HAB, NB_STA_ALL_HAB)
                {
                    AVANT = AVANT,
                    KHAB = KHAB,
                    KHSA = KHSA,
                    KHDI = KHDI,
                    APRES = APRES,
                    CHARIOT = CHARIOT,
                });
            }

            // FIN DE FICHIER
            Console.WriteLine("NB.GUS=", IG);

            return fushab09Indivs;
        }

        private int[,,] chab1qhp(int NBSTA, List<Fushab09Indiv> fushab09Indivs)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES HABITUDES 1 / 4h PAR INDIVIDU

            //  Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)

            int NBIND = fushab09Indivs.Count;
            int[,,] NINI = new int[NBIND + 1, 96 + 1, 3 + 1];
            int[] ISTA = new int[NBSTA + 1];


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

            // C On attribue 1 aux stations qui ne possèdent pas d'habitudes d'écoute.A mon avis, cela est maintenant
            // C inutile puisqu'on leur attribue leur notoriété.
            int[] ITH = new int[] {
                9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
                1,1,1,1,
                2,2,2,2,2,2,2,2,2,2,2,2,
                3,3,3,3,3,3,3,3,3,3,3,3,
                4,4,4,4,4,4,4,4,
                5,5,5,5,5,5,5,5,
                6,6,6,6,6,6,6,6,
                7,7,7,7,7,7,7,7,
                8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};

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
                            NINI[IG, IQ, IU] = 5 - IH;
                            if (ISTA[IPO] == 1 && IH != 5) NINI[IG, IQ, IU] = 1;
                        }
                    }
                }
            }

            return NINI;
        }

        private int[,] crenonin(int NBJOUR, int NBSTA, List<Fushab09Indiv> fushab09Indivs, VsorPoid[][] JN)
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
                IG = IG + 1;
                // BOUCLE STATIONS
                for (int NOP = 1; NOP <= NBSTA; NOP++)
                {
                    int IP = NOP;
                    int IPO = 6 * (IP - 1) + 1;
                    int NOSH = NOP;
                    KHI2[IG, NOP] = 0;

                    // NON INITIALISE ?
                    bool sortie = false;
                    if ((NOP != 20) && (NOP != 21) && (NOP != 22) && (NOP != 23) && (NOP != 24) && (NOP != 25) && (NOP != 26) && (NOP != 27))
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

                    // id  idx sta
                    // 16  101    Les Indés Radios 1
                    // 47  201    NRJ Global Proximité Premium 20
                    // 50  303    Nova and Friends 21
                    // 53  305    Lagardère Publicité News IDF 22
                    // 55  306    Lip !24
                    // 56  307    Paris – IDF + 25
                    // 60  310    Les Indés Capitale 27
                    // 61  311    TF1 Pub Radios 28

                    // Les Indés Radios
                    if (NOP == 1 && (indiv.APRES[1] - 48) == 1) sortie = true;
                    // NRJ Global Proximité Premium
                    if (NOP == 20 && (indiv.APRES[19] - 48) == 1) sortie = true;
                    // Nova and Friends
                    if (NOP == 21 && (indiv.APRES[28] - 48) == 1) sortie = true;
                    // Lagardère Publicité News IDF
                    if (NOP == 22 && (indiv.APRES[30] - 48) == 1) sortie = true;
                    // Lip !
                    if (NOP == 24 && (indiv.APRES[31] - 48) == 1) sortie = true;
                    // Paris – IDF +
                    if (NOP == 25 && (indiv.APRES[32] - 48) == 1) sortie = true;
                    // Les Indés Capitale
                    if (NOP == 27 && (indiv.APRES[35] - 48) == 1) sortie = true;
                    // TF1 Pub Radios
                    if (NOP == 28 && (indiv.APRES[36] - 48) == 1) sortie = true;

                    if (!sortie)
                    {
                        for (int I = 1; I <= NBJOUR; I++)
                        {
                            for (int IQ = 1; IQ <= 96; IQ++)
                            {
                                int[] bits = new int[7];
                                for (int b = 1; b <= 6; b++)
                                    bits[b] = JN[I][IG].VSor[b, NOP];

                                if (L1BITFCT(bits, IQ))
                                {
                                    sortie = true;
                                    break;
                                }
                            }
                        }
                        KHI2[IG, NOP] = 1;
                    }
                }
            }

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

        private void ecrsegpa(string pathSig, int COL_PIAB, int COL_CSCI, int COL_SEX, int COL_AGE, int COL_RUDA, int NBSTA, int NBIND, int SIGN_LINE_LEN_FULL,
            out int[,] SEGM, out int IPOP)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // ECRITURE DES SEGMENTS ET DU POIDS

            // PARAMETER(NBIND =#NB_INDIV#)

            // Longueur d'une ligne: 599 + 2 car de fin
            //int[] KHI2 = new int[SIGN_LINE_LEN_FULL + 1];
            SEGM = new int[NBIND + 1, 5 + 1];
            int ICSP, IREG, ISEX, IAGE, ISEG;
            int[] SEG2 = new int[16 + 1] { 999999, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 9, 10, 10 };
            int[] SEG3 = new int[10 + 1] { 999999, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6 };
            int[] SEG4 = new int[6 + 1] { 999999, 1, 2, 2, 3, 3, 4 };
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
                int IPERS = int.Parse(strIndiv.Substring(COL_PIAB, 5));

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
                    if ((strIndiv[COL_CSCI] == '1') || (int.Parse(strIndiv[COL_CSCI].ToString()) > 4)) ICSP = 2;
                    if (int.Parse(strIndiv[COL_CSCI].ToString()) > 6) ICSP = 3;

                    //--------------------
                    // Sexe de l'interviewé
                    //--------------------
                    // Le sexe de l'interviewé est codé sur une colonne (colonne #COL,SEXE#)
                    // ISEX = 1 correspond aux Hommes
                    // ISEX = 2 correspond aux Femmes
                    ISEX = int.Parse(strIndiv[COL_SEX].ToString());

                    //------------------ -
                    // Age de l'interviewé
                    //------------------ -
                    // L'Age de l'interviewé est codé sur deux colonnes(colonne #COL,AG11,0# et #COL,AG11,1#)
                    // IAGE = 1 correspond aux 13 - 34 ans
                    // IAGE = 2 correspond aux 35 - 59 ans
                    // IAGE = 3 correspond aux 60 ans et +
                    int AGE = int.Parse(strIndiv.Substring(COL_AGE, 2));
                    IAGE = 1;
                    if (AGE > 4) IAGE = 2;
                    if (AGE > 9) IAGE = 3;

                    //--------------------------
                    // Région UDA de l'interviewé
                    //--------------------------
                    // La région UDA de l'interviewé est codé sur une colonne (colonne #COL,RUDA#)
                    // IREG = 1 correspond aux 6 UDA Nord
                    // IREG = 2 correspond aux 3 UDA Sud

                    IREG = 1;
                    if (int.Parse(strIndiv[COL_RUDA].ToString()) > 6) IREG = 2;

                    //-------------------------------------------------------------------------- -
                    // On fonction des variables précédemment calculées, on détermine les segments
                    //-------------------------------------------------------------------------- -

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
                SEGM[IG, 2] = ISEG;
                SEGM[IG, 3] = SEG2[ISEG];
                SEGM[IG, 4] = SEG3[SEG2[ISEG]];
                SEGM[IG, 5] = SEG4[SEG3[SEG2[ISEG]]];

                for (int i = 1; i <= 16; i++)
                {
                    if (ISEG == i) C[i]++;
                }
            }

            for (int i = 1; i <= 16; i++)
            {
                Console.WriteLine(C[i]);
            }
        }

        private int[,,,] calcregr(int NBIND, int NBSTA, int[,] NINI, int[,] POIDSEGM, VsorPoid[][] JN)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CONTROLE DES MINIMA D'ECOUTES PAR CELLULES DE BASE          
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;
            int NBCEL = 23;
            int[,] KHI2 = new int[NBIND + 1, 6 + 1];
            int[] IAUD = new int[3 + 1];
            int[,,,] MINIS = new int[3 + 1, NBSTA + 1, 96 + 1, NBCEL + 1];

            // BOUCLE INDIVIDUS
            for (int IG = 0; IG <= NBIND; IG++)
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

                                int[] bits = new int[7];
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

        private int[,,,] calcnivo(int NBIND, int NBSTA, int[,,,] MINIS)
        {
            // C PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // REGROUPEMENTS DES CELLULES DE BASE
            int NBCEL = 16;
            int NIVREG = 9;

            // Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)
            int[,,,] REGRS = new int[3 + 1, NBSTA + 1, 96 + 1, NBCEL + 1];
            int[] TREG = new int[NBCEL];
            int[,] NIVO = new int[,]  { {0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
                                   {999999, 1,1,2,3,3,4,4,5,6,6,7,8,9,9,10,10 },
                                   {999999,-1,1,1,2,2,3,3,3,4,4,4,5,6,6,6,6},
                                   {999999,-1,1,1,2,2,2,2,2,3,3,3,3,4,4,4,4 },
                                   {999999,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 } };

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
                        if (N == 0)
                        {
                            for (int IC = 1; IC <= NBCEL; IC++)
                            {
                                TREG[IC] = 5;
                            }
                        }
                        else
                        {
                            // REGROUPEMENT NIVEAU N1 >> NIVEAU N2
                            bool sortie = false;
                            for (int IN = 1; IN <= 3; IN++)
                            {
                                for (int IC = 1; IC <= NBCEL; IC++)
                                {
                                    if (TREG[IC] == IN)
                                    {
                                        for (int I = 1; I <= NBCEL; I++)
                                        {

                                            if (NIVO[IN, I] == NIVO[IN, IC]) TREG[I] = TREG[IC];
                                        }
                                    }
                                }
                                N = 0;
                                for (int IC = 1; IC <= NBCEL; IC++)
                                {
                                    if (TREG[IC] == IN)
                                    {
                                        int IM = 0;

                                        for (int I = 1; I <= NBCEL; I++)
                                        {
                                            if (NIVO[IN, I] == NIVO[IN, IC]) IM = IM + MINIS[IU, IP, IQ, I];
                                        }

                                        if (IM < NIVREG)
                                        {
                                            N = 1;
                                            for (int I = 1; I <= NBCEL; I++)
                                            {
                                                if (NIVO[IN, I] == NIVO[IN, IC]) TREG[I] = IN + 1;
                                            }
                                        }
                                    }
                                }

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
                                    if (TREG[IC] == 4)
                                    {
                                        for (int I = 1; I <= NBCEL; I++)
                                        {
                                            TREG[I] = 4;
                                        }
                                    }
                                }
                            }
                        }
                        for (int IC = 1; IC <= NBCEL; IC++)
                        {
                            REGRS[IU, IP, IQ, IC] = TREG[IC];
                        }
                    }
                }
            }

            for (int I = 1; I <= NBCEL; I++)
                Console.WriteLine(REGRS[2, 2, 11, I]);
            for (int I = 1; I <= NBCEL; I++)
                Console.WriteLine(MINIS[2, 2, 11, I]);

            return REGRS;
        }

        // VsorPoid[][] JN [Jour 1..23][Individus 1..N] = {VOSR[,]?, Poid[]}
        // retour [Station, INdiv, Qh, 1..3]
        private int[,,,] caud1qhp(int NBIND, int NBSTA, VsorPoid[][] JN, int[,] POIDSEGM)
        {
            // C PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES AUDIENCES PAR INDIVIDU/ QUARTS d'HEURE/STATIONS
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;
            int[,,,] NIN2 = new int[NBSTA + 1, NBIND + 1, 96 + 1, 3 + 1];
            int[] NINI = new int[NBIND + 1];

            // OPEN(15, FILE = '#OUTPUT#audqhind',
            //-FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')

            for (int STA = 1; STA <= NBSTA; STA++)
            {
                // BOUCLE INDIVIDUS
                for (int IG = 0; IG <= NBIND; IG++)
                {

                    for (int IJ = 1; IJ <= NBJOUR; IJ++)
                    {
                        int IU = 1;
                        if (IJ == 1 || IJ == 8 || IJ == 15 || IJ == 22) IU = 2;
                        if (IJ == 2 || IJ == 9 || IJ == 16 || IJ == 23) IU = 3;

                        for (int IQ = 1; IQ <= 96; IQ++)
                        {

                            int[] bits = new int[7];
                            for (int b = 1; b <= 6; b++)
                                bits[b] = JN[IJ][IG].VSor[b, STA];

                            if (L1BITFCT(bits, IQ))
                            {
                                NIN2[STA, IG, IQ, IU] = NIN2[STA, IG, IQ, IU] + POIDSEGM[IQ, IJ];
                            }

                        }
                    }
                }
            }

            return NIN2;
        }

        private double[] caudtotp(int NBIND, int NBSTA, VsorPoid[][] JN, int[,] POIDS, List<Fushab09Indiv> fushab09Indivs)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES AUDIENCES TOTALES PAR INDIVIDUS / STATION
            // POUR DISTANCES ENTRE INDIVIDUS
            // Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)

            int NBJOUR = 23;

            // Dans les fichier .jfc, chaque enregistrement(individu) est de 180 short = 6 short(96 qh) * 30 stations

            double[] COUV = new double[NBIND + 1];
            double[] NOTE = new double[NBIND + 1];
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
                            int[] bits = new int[7];
                            for (int b = 1; b <= 6; b++)
                                bits[b] = JN[IJ][IG].VSor[b, IPO];
                            if (L1BITFCT(bits, IQ))
                            {
                                COUV[IG] = COUV[IG] + JN[IJ][IG].Poid[IQ];
                            }
                        }
                    }
                }

                //  **CRITERES**
                // SEXE
                NINI[IG, 1] = fushab09Indiv.AVANT[14] - 48;
                // AGE
                int AGE = 10 * (fushab09Indiv.AVANT[24] - 48) + (fushab09Indiv.AVANT[25] - 48);
                NINI[IG, 2] = 1;
                if (AGE > 2) NINI[IG, 2] = 2;
                if (AGE > 4) NINI[IG, 2] = 3;
                if (AGE > 7) NINI[IG, 2] = 4;
                if (AGE > 10) NINI[IG, 2] = 5;
                // AGE
                NINI[IG, 3] = 1;
                if (AGE > 7) NINI[IG, 3] = 2;
                // AGE
                NINI[IG, 4] = 1;
                if (AGE > 1) NINI[IG, 4] = 2;
                // Profession Individu
                NINI[IG, 5] = fushab09Indiv.AVANT[18] - 48;
                // Région UDA
                NINI[IG, 6] = fushab09Indiv.AVANT[20] - 48;
                // Région UDA
                NINI[IG, 7] = 1;
                if ((fushab09Indiv.AVANT[20] - 48) > 1) NINI[IG, 7] = 2;
                if ((fushab09Indiv.AVANT[20] - 48) > 6) NINI[IG, 7] = 3;
                // Habitat
                NINI[IG, 8] = 1;
                if ((fushab09Indiv.AVANT[17] - 48) > 1) NINI[IG, 8] = 2;
                if ((fushab09Indiv.AVANT[17] - 48) > 2) NINI[IG, 8] = 3;
                if ((fushab09Indiv.AVANT[17] - 48) > 4) NINI[IG, 8] = 4;
                if ((fushab09Indiv.AVANT[17] - 48) > 6) NINI[IG, 8] = 5;
                // Ménagère
                NINI[IG, 9] = (fushab09Indiv.AVANT[15] - 48);
                // Responsable des achats
                NINI[IG, 10] = (fushab09Indiv.AVANT[16] - 48);
                // Profession du chef de famille
                NINI[IG, 11] = (fushab09Indiv.AVANT[19] - 48);
                // Région NIELSEN
                NINI[IG, 12] = (fushab09Indiv.AVANT[21] - 48);
                // Région INSEE
                int INSEE = 10 * (fushab09Indiv.AVANT[22] - 48) + (fushab09Indiv.AVANT[23] - 48);
                NINI[IG, 13] = INSEE;
                // Nb d'enfants de moins de 6 ans
                NINI[IG, 14] = fushab09Indiv.AVANT[27] - 48;
                // Nb d'enfants de 6 à 8 ans
                NINI[IG, 15] = fushab09Indiv.AVANT[28] - 48;
                // Nb d'enfant de 9 à 10 ans
                NINI[IG, 16] = fushab09Indiv.AVANT[29] - 48;
                // Nb d'enfant de 11 à 14 ans
                NINI[IG, 17] = fushab09Indiv.AVANT[30] - 48;
                // Nb de personnes vivant dans le foyer
                NINI[IG, 18] = fushab09Indiv.AVANT[31] - 48;
                // Etape de la vie
                int ETAPE = 10 * (fushab09Indiv.APRES[11] - 48) + fushab09Indiv.APRES[12] - 48;
                NINI[IG, 19] = ETAPE;
                Console.WriteLine(ETAPE);
                // Cellule
                int CELLULE = 10 * (fushab09Indiv.AVANT[33] - 48) + (fushab09Indiv.AVANT[34] - 48);
                NINI[IG, 20] = CELLULE;
            }

            // CALCUL DE LA NOTE INDIVIDUELLE
            double MINOT = 100000;
            double MANOT = 0;
            for (int I = 1; I <= NBIND; I++)
            {
                double NUM = 0d;
                double DEN = 0d;

                for (int J = 1; J <= NBIND; J++)
                {
                    int DIF = 0;
                    int NO = 25;
                    for (int IC = 1; IC <= 20; IC++)
                    {
                        if (IC == 9) NO = 1;
                        if (NINI[I, IC] != NINI[J, IC]) DIF = DIF + NO;
                    }
                    double D = -1d * ((DIF / 25) ^ 2);
                    double A = POIDS[J, 1] * Math.Exp(D);   // A VOIR POIDS[J,1] : à priori on ne lirait que la premire zone de poiids dans fichier origine => tableau 1 dimension

                    DEN = DEN + A;
                    NUM = NUM + A * (COUV[J] / 12d);
                }

                NOTE[I] = NUM / DEN;
            }

            for (int I = 1; I <= NBIND; I++)
            {
                if (NOTE[I] < MINOT) MINOT = NOTE[I];

                if (NOTE[I] > MANOT) MANOT = NOTE[I];
            }

            Console.WriteLine($"MINOT {MINOT} MANOT {MANOT}");

            return NOTE;
        }

        private double[,,,] sav1qhpa(int NBIND, int NBSTA, int[,,,] regrs, int[,] KHI2, List<Fushab09Indiv> fushab09Indivs, VsorPoid[][][] JNByWeek, VsorPoid[][] JN)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES PARAMETRES STATION 2 QUART d'HEURE 31
            //(EUROPE 1 - 7H30 - 7H45)
            // LUNDI-VENDREDI
            // VERSION 2
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;

            double[,] C = new double[15 + 1, 5 + 1];
            double[] NCOM = new double[15 + 1] { 999999d, 15d, 105d, 455d, 1365d, 3003d, 5005d, 6435d, 6435d, 5005d, 3003d, 1365d, 455d, 105d, 15d, 1d };
            double[,,,] RESUL = new double[NBSTA + 1, 96 + 1, 4 + 1, 16 + 1];
            double[] Z = new double[15 + 1];
            double[] YR = new double[15 + 1];

            int[,] COMPT = new int[16 + 1, 5 + 1];
            int[] ITPO = new int[5 + 1];
            int[] ITTP = new int[5 + 1];
            int[] IZAP = new int[5 + 1];
            int[] IPPS = new int[5 + 1];
            int[] ITJP = new int[5 + 1];
            int[,] SEGM = new int[16 + 1, 5 + 1];
            int[] NB = new int[5 + 1];
            int[] ITTL = new int[5 + 1];
            int[] ITOU = new int[5 + 1];
            int[] SEG2 = new int[16 + 1];
            int[] SEG3 = new int[10 + 1];
            int[] SEG4 = new int[6 + 1];
            int[] COMB = new int[15 + 1];
            int[] IZAB = new int[5 + 1];
            int[] ITJR = new int[5 + 1];
            int[] IM = new int[5 + 1];
            //int[,] POIQH = new int[96 + 1, NBJOUR + 1];
            int[,,] PATRON = new int[96 + 1, 15 + 1, NBIND + 1];
            int[,,] JATOU = new int[96 + 1, 4 + 1, NBIND + 1];
            int[,,] REGRS = new int[3 + 1, NBSTA + 1, 16 + 1];
            int[] TREG = new int[16 + 1];

            int[] ITH = new int[96 + 1];
            int[] ITHInit = new int[] { 999999, 20 * 9, 4 * 1, 12 * 2, 12 * 3, 8 * 4, 8 * 5, 8 * 6, 8 * 7, 16 * 8 };
            for (int i = 0; i < ITHInit.Length; i++) ITH[i] = ITHInit[i];

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

            StringBuilder sbSortie = new StringBuilder();

            // OPEN(18, FILE = '#OUTPUT#ZUPTAUSE',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')


            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
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
                        int IH = fushab09Indiv.KHAB[ITH[IQ], NOSH] - 48;
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
                        for (int I = 1; I <= 16; I++)
                        {
                            TREG[I] = regrs[1, IP, IQ, I];
                        }

                        if (TREG[1] == 5) continue;
                        int IJK = 1;
                        for (int I = 3; I <= 21; I++)
                        {
                            if (I == 8 || I == 9 || I == 15 || I == 16) continue;

                            int[] bits = new int[7];
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
                        if (NJOU == 15) JATOU[IQ, 3, IG] = 1;
                        if (NJOU == 15 && IH == 1) JATOU[IQ, 4, IG] = 1;
                    } //  for (int IQ = 1; IQ <= 96; IQ++)
                } // foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)

                // 120 
                // BOUCLE 1 / 4h
                bool noAudience = false;
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    sbSortie.Append($"IQ = {IQ}");
                    Console.WriteLine($"--------STATION {NOP} --------1/4h {IQ} ");
                    for (int I = 1; I <= 16; I++)
                    {
                        TREG[I] = regrs[1, IP, IQ, I];
                    }

                    if (TREG[1] != 5)
                    {

                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            double ZR = 0d;
                            double UR = 0d;
                            double PR = 0d;
                            double TAU = 0d;

                            int N2 = SEG1[N1];
                            int N3 = SEG2[N2];
                            int N4 = SEG3[N3];
                            NB[1] = N1;
                            NB[2] = N2;
                            NB[3] = N3;
                            NB[4] = N4;
                            NB[5] = 1;
                            int NIV = TREG[N1] + 1;
                            if (((N1 == 1) || (NIV == 1))
                                ||
                               (!((NIV == 2 && SEG1[N1] == SEG1[N1 - 1]) ||
                                  (NIV == 3 && SEG2[SEG1[N1]] == SEG2[SEG1[N1 - 1]]) ||
                                  (NIV == 4 && SEG3[SEG2[SEG1[N1]]] == SEG3[SEG2[SEG1[N1 - 1]]]) ||
                                  (NIV == 5))))
                            {
                                for (int I = 1; I <= 16; I++)
                                {
                                    for (int J = 1; J <= 5; J++)
                                    {
                                        SEGM[I, J] = 0;
                                        COMPT[I, J] = 0;
                                        if (I <= 15) C[I, J] = 0d;
                                    }
                                }

                                for (int J = 1; J <= 5; J++)
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
                                    KHI2[IG, 6] = 1;
                                    IPERS = IPERS * 10;
                                    if (KHI2[IG, NIV + 1] == NB[NIV]) IM[NIV] = IM[NIV] + 1;
                                    if (KHI2[IG, NIV + 1] == NB[NIV]) IPPS[NIV] = IPPS[NIV] + IPERS;
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
                                        if (KHI2[IG, NIV + 1] == NB[NIV])
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
                                        if (KHI2[IG, NIV + 1] == NB[NIV])
                                        {
                                            COMPT[IJ, NIV] = COMPT[IJ, NIV] + IPERS;
                                            SEGM[IJ, NIV] = SEGM[IJ, NIV] + 1;

                                            ITOU[NIV] = ITOU[NIV] + 1;
                                            ITPO[NIV] = ITPO[NIV] + IPERS;
                                        }
                                        if (NJOU == 15)
                                        {
                                            if (KHI2[IG, NIV + 1] == NB[NIV])
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

                                        if (KHI2[IG, NIV + 1] == NB[NIV])
                                        {
                                            #region CALCUL DES MOYENNES

                                            // PREMIER JOUR
                                            double TOT = 0d;
                                            for (int I01 = 1; I01 <= 15; I01++)
                                                if (COMB[I01] == 1) TOT += IPERS;
                                            C[1, NIV] += TOT;

                                            // 2eme JOUR
                                            TOT = 0d;
                                            for (int I01 = 1; I01 <= 14; I01++)
                                                for (int I02 = I01 + 1; I02 <= 15; I02++)
                                                    if (COMB[I01] == 1 || COMB[I02] == 1) TOT += IPERS;
                                            C[2, NIV] += TOT;

                                            // 3eme JOUR
                                            TOT = 0d;
                                            for (int I01 = 1; I01 <= 13; I01++)
                                                for (int I02 = I01 + 1; I02 <= 14; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 15; I03++)
                                                        if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1) TOT += IPERS;

                                            C[3, NIV] += TOT;

                                            // 4eme JOUR
                                            TOT = 0d;
                                            for (int I01 = 1; I01 <= 12; I01++)
                                                for (int I02 = I01 + 1; I02 <= 13; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 14; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 15; I04++)
                                                            if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                COMB[I04] == 1) TOT += IPERS;
                                            C[4, NIV] += TOT;

                                            // 5eme JOUR
                                            TOT = 0d;
                                            for (int I01 = 1; I01 <= 11; I01++)
                                                for (int I02 = I01 + 1; I02 <= 12; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 13; I03++)
                                                        for (int I04 = I03 + 1; I04 <= 14; I04++)
                                                            for (int I05 = I04 + 1; I05 <= 15; I05++)
                                                                if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1 ||
                                                                    COMB[I04] == 1 || COMB[I05] == 1) TOT += IPERS;
                                            C[5, NIV] += TOT;

                                            // 6eme JOUR
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                            TOT = 0d;
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
                                double POPS = 0d;
                                for (int I = 1; I <= NBIND; I++)
                                {
                                    if (KHI2[I, IN + 1] == NB[IN]) POPS = POPS + KHI2[I, 1];
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

                                for (int I = 2; I <= 16; I++)
                                {
                                    if (I != 16) SEGM[I + 1, IN] = SEGM[I + 1, IN] + SEGM[I, IN];
                                    if (I != 16) COMPT[I + 1, IN] = COMPT[I + 1, IN] + COMPT[I, IN];
                                }

                                double GRP = IPOP * 100;
                                GRP = GRP / POPS / 15d;
                                PR = (IPOP - ITTP[IN] * 15) / 14d;
                                PR = PR / (POPS - COMPT[1, IN] - ITTP[IN]);
                                for (int I = 1; I <= 15; I++)
                                {
                                    Z[I] = C[I, IN] / NCOM[I] / POPS * 100d;
                                }

                                // APPEL DE LA FONCTION DIFFERENCE

                                GRP = GRP / 100d;
                                double ZA = DIRAC0;
                                ZR = IZAP[IN] / POPS;
                                double UA = ITJP[IN] / POPS;
                                UR = DIRAC1;

                                //WRITE(16, *) GRP, ZA, ZR, UA, UR
                                sbSortie.Append("{GRP}  {ZA}  {ZR}  {UA}  {UR}" + $"{GRP}  {ZA}  {ZR}  {UA}  {UR}");

                                double ND = 15;
                                MINIMISE(GRP, Z, ref PR, ref UR, ref ZR, out TAU, ZA, UA, ND); // double GRP, double[] ZC, ref double PR, ref double UR, ref double ZR, out double TAU, double ZA, double UA, double NB)

                                double GRP0 = 0d;
                                double GRPC = 0d;
                                for (int I = 1; I <= 15; I++)
                                {
                                    GRPC = GRPC + YR[I];
                                    GRP0 = GRP0 + Z[I];
                                }
                            } // if (((N1 == 1) || (NIV == 1) ) 
                            RESUL[NOP, IQ, 1, N1] = ZR;
                            RESUL[NOP, IQ, 2, N1] = UR;
                            RESUL[NOP, IQ, 3, N1] = PR;
                            RESUL[NOP, IQ, 4, N1] = TAU;

                            sbSortie.Append("N1 = " + N1.ToString());
                            sbSortie.Append("ZR = " + ZR.ToString());
                            sbSortie.Append("UR = " + UR.ToString());
                            sbSortie.Append("PR = " + PR.ToString());
                            sbSortie.Append("TAU = " + TAU.ToString());

                        } // for (int N1 = 1; N1 <= 16; N1++)
                    }
                    else
                    {
                        sbSortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            RESUL[NOP, IQ, 1, N1] = 1d;
                            RESUL[NOP, IQ, 2, N1] = 0d;
                            RESUL[NOP, IQ, 3, N1] = 0d;
                            RESUL[NOP, IQ, 4, N1] = 0d;
                        }
                    }// if (TREG[1] != 5)
                    // FIN DE TRAITEMENT CELLULE
                } // for (int IQ = 1; IQ <= 96; IQ++)
            } // for (int NOP = 1; NOP <= NBSTA; NOP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");
            //FORMAT('CELLULE ',I2,' Z=',F9.7,' U=',F9.7,' P=',F9.7,' TAU=',F9.7) == ??? 
            //? Console.WriteLine($"CELLULE {I2} Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7)
            return RESUL;
        }

        private double[,,,] sav1qhps(int NBIND, int NBSTA, int[,,,] regrs, int[,] KHI2, List<Fushab09Indiv> fushab09Indivs, VsorPoid[][][] JNByWeek, VsorPoid[][] JN)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES PARAMETRES z,u,p,tau
            // SAMEDI
            // VERSION 2
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;

            double[,] C = new double[15 + 1, 5 + 1];
            double[] NCOM = new double[4 + 1] { 999999d, 4, 6, 4, 1 };
            double[,,,] RESUL = new double[NBSTA + 1, 96 + 1, 4 + 1, 16 + 1];
            double[] Z = new double[15 + 1];
            double[] YR = new double[15 + 1];

            int[,] COMPT = new int[16 + 1, 5 + 1];
            int[] ITPO = new int[5 + 1];
            int[] ITTP = new int[5 + 1];
            int[] IZAP = new int[5 + 1];
            int[] IPPS = new int[5 + 1];
            int[] ITJP = new int[5 + 1];
            int[,] SEGM = new int[16 + 1, 5 + 1];
            int[] NB = new int[5 + 1];
            int[] ITTL = new int[5 + 1];
            int[] ITOU = new int[5 + 1];
            int[] COMB = new int[15 + 1];
            int[] IZAB = new int[5 + 1];
            int[] ITJR = new int[5 + 1];
            int[] IM = new int[5 + 1];
            //int[,] POIQH = new int[96 + 1, NBJOUR + 1];
            int[,,] PATRON = new int[96 + 1, 15 + 1, NBIND + 1];
            int[,,] JATOU = new int[96 + 1, 4 + 1, NBIND + 1];
            int[] TREG = new int[16 + 1];

            int[] ITH = new int[96 + 1];
            int[] ITHInit = new int[] { 999999, 20 * 9, 4 * 1, 12 * 2, 12 * 3, 8 * 4, 8 * 5, 8 * 6, 8 * 7, 16 * 8 };
            for (int i = 0; i < ITHInit.Length; i++) ITH[i] = ITHInit[i];

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

            StringBuilder sbSortie = new StringBuilder();

            // OPEN(18, FILE = '#OUTPUT#ZUPTAUSA',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')


            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
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
                        int IH = fushab09Indiv.KHSA[ITH[IQ], NOSH] - 48;
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
                        for (int I = 1; I <= 16; I++)
                        {
                            TREG[I] = regrs[2, IP, IQ, I];
                        }

                        if (TREG[1] == 5) continue;
                        int IJK = 1;
                        for (int I = 1; I <= 22; I += 7)
                        {
                            int[] bits = new int[7];
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
                    sbSortie.Append($"IQ = {IQ}");
                    Console.WriteLine($"--------STATION {NOP} --------1/4h {IQ} ");
                    for (int I = 1; I <= 16; I++)
                    {
                        TREG[I] = regrs[2, IP, IQ, I];
                    }

                    if (TREG[1] != 5)
                    {

                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            double ZR = 0d;
                            double UR = 0d;
                            double PR = 0d;
                            double TAU = 0d;

                            int N2 = SEG1[N1];
                            int N3 = SEG2[N2];
                            int N4 = SEG3[N3];
                            NB[1] = N1;
                            NB[2] = N2;
                            NB[3] = N3;
                            NB[4] = N4;
                            NB[5] = 1;
                            int NIV = TREG[N1] + 1;
                            if (((N1 == 1) || (NIV == 1))
                                ||
                               (!((NIV == 2 && SEG1[N1] == SEG1[N1 - 1]) ||
                                  (NIV == 3 && SEG2[SEG1[N1]] == SEG2[SEG1[N1 - 1]]) ||
                                  (NIV == 4 && SEG3[SEG2[SEG1[N1]]] == SEG3[SEG2[SEG1[N1 - 1]]]) ||
                                  (NIV == 5))))
                            {
                                for (int I = 1; I <= 16; I++)
                                {
                                    for (int J = 1; J <= 5; J++)
                                    {
                                        SEGM[I, J] = 0;
                                        COMPT[I, J] = 0;
                                        if (I <= 4) C[I, J] = 0d;
                                    }
                                }

                                for (int J = 1; J <= 5; J++)
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
                                    KHI2[IG, 6] = 1;
                                    IPERS = IPERS * 10;
                                    if (KHI2[IG, NIV + 1] == NB[NIV]) IM[NIV] = IM[NIV] + 1;
                                    if (KHI2[IG, NIV + 1] == NB[NIV]) IPPS[NIV] = IPPS[NIV] + IPERS;
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
                                        if (KHI2[IG, NIV + 1] == NB[NIV])
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
                                        if (KHI2[IG, NIV + 1] == NB[NIV])
                                        {
                                            COMPT[IJ, NIV] = COMPT[IJ, NIV] + IPERS;
                                            SEGM[IJ, NIV] = SEGM[IJ, NIV] + 1;

                                            ITOU[NIV] = ITOU[NIV] + 1;
                                            ITPO[NIV] = ITPO[NIV] + IPERS;
                                        }
                                        if (NJOU == 4)
                                        {
                                            if (KHI2[IG, NIV + 1] == NB[NIV])
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

                                        if (KHI2[IG, NIV + 1] == NB[NIV])
                                        {
                                            #region CALCUL DES MOYENNES

                                            // PREMIER JOUR
                                            double TOT = 0d;
                                            for (int I01 = 1; I01 <= 4; I01++)
                                                if (COMB[I01] == 1) TOT += IPERS;
                                            C[1, NIV] += TOT;

                                            // 2eme JOUR
                                            TOT = 0d;
                                            for (int I01 = 1; I01 <= 3; I01++)
                                                for (int I02 = I01 + 1; I02 <= 4; I02++)
                                                    if (COMB[I01] == 1 || COMB[I02] == 1) TOT += IPERS;
                                            C[2, NIV] += TOT;

                                            // 3eme JOUR
                                            TOT = 0d;
                                            for (int I01 = 1; I01 <= 2; I01++)
                                                for (int I02 = I01 + 1; I02 <= 3; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 4; I03++)
                                                        if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1) TOT += IPERS;
                                            C[3, NIV] += TOT;

                                            // 4eme JOUR
                                            TOT = 0d;
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
                                    if (KHI2[I, IN + 1] == NB[IN]) POPS = POPS + KHI2[I, 1];
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
                                    if (I != 5) SEGM[I + 1, IN] = SEGM[I + 1, IN] + SEGM[I, IN];
                                    if (I != 5) COMPT[I + 1, IN] = COMPT[I + 1, IN] + COMPT[I, IN];
                                }

                                double GRP = IPOP * 100;
                                GRP = GRP / POPS / 4d;
                                PR = (IPOP - ITTP[IN] * 4d) / 3d;
                                if (POPS == (COMPT[1, IN] + ITTP[IN]))
                                {
                                    ZR = COMPT[1, IN] / POPS;
                                    UR = ITTP[IN] / POPS;
                                    PR = 1d;
                                    TAU = 0d;
                                }
                                else
                                {
                                    PR = PR / (POPS - COMPT[1, IN] - ITTP[IN]);
                                    for (int I = 1; I <= 4; I++)
                                    {
                                        Z[I] = C[I, IN] / NCOM[I] / POPS * 100d;
                                    }

                                    // APPEL DE LA FONCTION DIFFERENCE

                                    GRP = GRP / 100d;
                                    double ZA = DIRAC0;
                                    ZR = IZAP[IN] / POPS;
                                    double UA = ITJP[IN] / POPS;
                                    UR = DIRAC1;

                                    //WRITE(16, *) GRP, ZA, ZR, UA, UR
                                    sbSortie.Append("{GRP}  {ZA}  {ZR}  {UA}  {UR}" + $"{GRP}  {ZA}  {ZR}  {UA}  {UR}");

                                    double ND = 4;
                                    MINIMISE(GRP, Z, ref PR, ref UR, ref ZR, out TAU, ZA, UA, ND, qhps: true); // double GRP, double[] ZC, ref double PR, ref double UR, ref double ZR, out double TAU, double ZA, double UA, double NB)
                                }
                            } // if (((N1 == 1) || (NIV == 1) ) 
                            RESUL[NOP, IQ, 1, N1] = ZR;
                            RESUL[NOP, IQ, 2, N1] = UR;
                            RESUL[NOP, IQ, 3, N1] = PR;
                            RESUL[NOP, IQ, 4, N1] = TAU;

                            sbSortie.Append("N1 = " + N1.ToString());
                            sbSortie.Append("ZR = " + ZR.ToString());
                            sbSortie.Append("UR = " + UR.ToString());
                            sbSortie.Append("PR = " + PR.ToString());
                            sbSortie.Append("TAU = " + TAU.ToString());

                        } // for (int N1 = 1; N1 <= 16; N1++)
                    }
                    else
                    {
                        sbSortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            RESUL[NOP, IQ, 1, N1] = 1d;
                            RESUL[NOP, IQ, 2, N1] = 0d;
                            RESUL[NOP, IQ, 3, N1] = 0d;
                            RESUL[NOP, IQ, 4, N1] = 0d;
                        }
                    }
                    // FIN DE TRAITEMENT CELLULE
                    // 1000 

                } // for (int IQ = 1; IQ <= 96; IQ++)
                  // 1001 
                  // 900  
            } // for (int NOP = 1; NOP <= NBSTA; NOP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            //   11       
            Console.WriteLine($"------- STATION ----");
            //    1 
            //FORMAT('CELLULE ',I2,' Z=',F9.7,' U=',F9.7,' P=',F9.7,' TAU=',F9.7) == ??? 
            //? Console.WriteLine($"CELLULE {I2} Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7)
            return RESUL;
        }

        private double[,,,] sav1qhpd(int NBIND, int NBSTA, int[,,,] regrs, int[,] KHI2, List<Fushab09Indiv> fushab09Indivs, VsorPoid[][][] JNByWeek, VsorPoid[][] JN)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES PARAMETRES z,u,p,tau
            // SAMEDI
            // VERSION 2
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int NBJOUR = 23;

            double[,] C = new double[15 + 1, 5 + 1];
            double[] NCOM = new double[4 + 1] { 999999d, 4, 6, 4, 1 };
            double[,,,] RESUL = new double[NBSTA + 1, 96 + 1, 4 + 1, 16 + 1];
            double[] Z = new double[15 + 1];
            double[] YR = new double[15 + 1];

            int[,] COMPT = new int[16 + 1, 5 + 1];
            int[] ITPO = new int[5 + 1];
            int[] ITTP = new int[5 + 1];
            int[] IZAP = new int[5 + 1];
            int[] IPPS = new int[5 + 1];
            int[] ITJP = new int[5 + 1];
            int[,] SEGM = new int[16 + 1, 5 + 1];
            int[] NB = new int[5 + 1];
            int[] ITTL = new int[5 + 1];
            int[] ITOU = new int[5 + 1];
            int[] SEG2 = new int[16 + 1];
            int[] SEG3 = new int[10 + 1];
            int[] SEG4 = new int[6 + 1];
            int[] COMB = new int[15 + 1];
            int[] IZAB = new int[5 + 1];
            int[] ITJR = new int[5 + 1];
            int[] IM = new int[5 + 1];
            //int[,] POIQH = new int[96 + 1, NBJOUR + 1];
            int[,,] PATRON = new int[96 + 1, 15 + 1, NBIND + 1];
            int[,,] JATOU = new int[96 + 1, 4 + 1, NBIND + 1];
            int[] TREG = new int[16 + 1];

            int[] ITH = new int[96 + 1];
            int[] ITHInit = new int[] { 999999, 20 * 9, 4 * 1, 12 * 2, 12 * 3, 8 * 4, 8 * 5, 8 * 6, 8 * 7, 16 * 8 };
            for (int i = 0; i < ITHInit.Length; i++) ITH[i] = ITHInit[i];

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

            StringBuilder sbSortie = new StringBuilder();

            // OPEN(18, FILE = '#OUTPUT#ZUPTAUSA',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')


            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
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
                        for (int I = 1; I <= 16; I++)
                        {
                            TREG[I] = regrs[3, IP, IQ, I];
                        }

                        if (TREG[1] == 5) continue;
                        int IJK = 1;
                        for (int I = 2; I <= 23; I += 7)
                        {
                            int[] bits = new int[7];
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
                    sbSortie.Append($"IQ = {IQ}");
                    Console.WriteLine($"--------STATION {NOP} --------1/4h {IQ} ");
                    for (int I = 1; I <= 16; I++)
                    {
                        TREG[I] = regrs[3, IP, IQ, I];
                    }

                    if (TREG[1] != 5)
                    {

                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            double ZR = 0d;
                            double UR = 0d;
                            double PR = 0d;
                            double TAU = 0d;

                            int N2 = SEG1[N1];
                            int N3 = SEG2[N2];
                            int N4 = SEG3[N3];
                            NB[1] = N1;
                            NB[2] = N2;
                            NB[3] = N3;
                            NB[4] = N4;
                            NB[5] = 1;
                            int NIV = TREG[N1] + 1;
                            if (((N1 == 1) || (NIV == 1))
                                ||
                               (!((NIV == 2 && SEG2[N1] == SEG2[N1 - 1]) ||
                                  (NIV == 3 && SEG3[SEG2[N1]] == SEG3[SEG2[N1 - 1]]) ||
                                  (NIV == 4 && SEG4[SEG3[SEG2[N1]]] == SEG4[SEG3[SEG2[N1 - 1]]]) ||
                                  (NIV == 5))))
                            {
                                for (int I = 1; I <= 16; I++)
                                {
                                    for (int J = 1; J <= 5; J++)
                                    {
                                        SEGM[I, J] = 0;
                                        COMPT[I, J] = 0;
                                        if (I <= 4) C[I, J] = 0d;
                                    }
                                }

                                for (int J = 1; J <= 5; J++)
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
                                    KHI2[IG, 6] = 1;
                                    IPERS = IPERS * 10;
                                    if (KHI2[IG, NIV + 1] == NB[NIV]) IM[NIV] = IM[NIV] + 1;
                                    if (KHI2[IG, NIV + 1] == NB[NIV]) IPPS[NIV] = IPPS[NIV] + IPERS;
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
                                        if (KHI2[IG, NIV + 1] == NB[NIV])
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
                                        if (KHI2[IG, NIV + 1] == NB[NIV])
                                        {
                                            COMPT[IJ, NIV] = COMPT[IJ, NIV] + IPERS;
                                            SEGM[IJ, NIV] = SEGM[IJ, NIV] + 1;

                                            ITOU[NIV] = ITOU[NIV] + 1;
                                            ITPO[NIV] = ITPO[NIV] + IPERS;
                                        }
                                        if (NJOU == 4)
                                        {
                                            if (KHI2[IG, NIV + 1] == NB[NIV])
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

                                        if (KHI2[IG, NIV + 1] == NB[NIV])
                                        {
                                            #region CALCUL DES MOYENNES

                                            // PREMIER JOUR
                                            double TOT = 0d;
                                            for (int I01 = 1; I01 <= 4; I01++)
                                                if (COMB[I01] == 1) TOT += IPERS;
                                            C[1, NIV] += TOT;

                                            // 2eme JOUR
                                            TOT = 0d;
                                            for (int I01 = 1; I01 <= 3; I01++)
                                                for (int I02 = I01 + 1; I02 <= 4; I02++)
                                                    if (COMB[I01] == 1 || COMB[I02] == 1) TOT += IPERS;
                                            C[2, NIV] += TOT;

                                            // 3eme JOUR
                                            TOT = 0d;
                                            for (int I01 = 1; I01 <= 2; I01++)
                                                for (int I02 = I01 + 1; I02 <= 3; I02++)
                                                    for (int I03 = I02 + 1; I03 <= 4; I03++)
                                                        if (COMB[I01] == 1 || COMB[I02] == 1 || COMB[I03] == 1) TOT += IPERS;
                                            C[3, NIV] += TOT;

                                            // 4eme JOUR
                                            TOT = 0d;
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
                                    if (KHI2[I, IN + 1] == NB[IN]) POPS = POPS + KHI2[I, 1];
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
                                    if (I != 5) SEGM[I + 1, IN] = SEGM[I + 1, IN] + SEGM[I, IN];
                                    if (I != 5) COMPT[I + 1, IN] = COMPT[I + 1, IN] + COMPT[I, IN];
                                }

                                double GRP = IPOP * 100;
                                GRP = GRP / POPS / 4d;
                                PR = (IPOP - ITTP[IN] * 4d) / 3d;
                                if (POPS == (COMPT[1, IN] + ITTP[IN]))
                                {
                                    ZR = COMPT[1, IN] / POPS;
                                    UR = ITTP[IN] / POPS;
                                    PR = 1d;
                                    TAU = 0d;
                                }
                                else
                                {
                                    PR = PR / (POPS - COMPT[1, IN] - ITTP[IN]);
                                    for (int I = 1; I <= 4; I++)
                                    {
                                        Z[I] = C[I, IN] / NCOM[I] / POPS * 100d;
                                    }

                                    // APPEL DE LA FONCTION DIFFERENCE

                                    GRP = GRP / 100d;
                                    double ZA = DIRAC0;
                                    ZR = IZAP[IN] / POPS;
                                    double UA = ITJP[IN] / POPS;
                                    UR = DIRAC1;

                                    //WRITE(16, *) GRP, ZA, ZR, UA, UR
                                    sbSortie.Append("{GRP}  {ZA}  {ZR}  {UA}  {UR}" + $"{GRP}  {ZA}  {ZR}  {UA}  {UR}");

                                    double ND = 4;
                                    MINIMISE(GRP, Z, ref PR, ref UR, ref ZR, out TAU, ZA, UA, ND); // double GRP, double[] ZC, ref double PR, ref double UR, ref double ZR, out double TAU, double ZA, double UA, double NB)
                                }
                            } // if (((N1 == 1) || (NIV == 1) ) 
                            RESUL[NOP, IQ, 1, N1] = ZR;
                            RESUL[NOP, IQ, 2, N1] = UR;
                            RESUL[NOP, IQ, 3, N1] = PR;
                            RESUL[NOP, IQ, 4, N1] = TAU;

                            sbSortie.Append("N1 = " + N1.ToString());
                            sbSortie.Append("ZR = " + ZR.ToString());
                            sbSortie.Append("UR = " + UR.ToString());
                            sbSortie.Append("PR = " + PR.ToString());
                            sbSortie.Append("TAU = " + TAU.ToString());

                        } // for (int N1 = 1; N1 <= 16; N1++)
                    } // if (TREG[1] != 5)
                    else
                    {
                        sbSortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            RESUL[NOP, IQ, 1, N1] = 1d;
                            RESUL[NOP, IQ, 2, N1] = 0d;
                            RESUL[NOP, IQ, 3, N1] = 0d;
                            RESUL[NOP, IQ, 4, N1] = 0d;
                        }
                    }
                } // for (int IQ = 1; IQ <= 96; IQ++)
            } // for (int NOP = 1; NOP <= NBSTA; NOP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");
            //FORMAT('CELLULE ',I2,' Z=',F9.7,' U=',F9.7,' P=',F9.7,' TAU=',F9.7) == ??? 
            //? Console.WriteLine($"CELLULE {I2} Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7)
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
        private double[,,,] cgrp75br(string PathGRPWave, int NbStation, int NbGRPModulation, int NbGRPStation, int[] NotorieteStation)
        {
            if (NbStation == 0
                || NbGRPModulation == 0
                || NbGRPStation == 0
                || NotorieteStation == null)
                return null;


            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES GRP 75000 POUR CALAGE

            double[,,,] COUV = new double[37 + 1, 3 + 1, NbStation + 1, 96 + 1];
            int[,] IPOP = new int[37 + 1, 3 + 1];

            int[] KHI2 = new int[NbGRPModulation + 1];
            int[] ISTA = new int[NbStation + 1];
            int ICSP, IREG, ISEX, IAGE, ISEG, COMPT;
            int[,] ZLEC = new int[96 + 1, NbGRPStation + 1];



            //   id idx    sta
            //   16  101    Les IndÃ©s Radios
            //   21  001    ChÃ©rie FM
            //   22  002    Europe 1
            //   23  005    Virgin Radio
            //   24  004    France Culture
            //   25  006    France Info
            //   26  008    France Inter
            //   27  007    France Musique
            //   28  009    Fun Radio
            //   29  010    M Radio
            //   30  011    Nostalgie
            //   31  012    NRJ
            //   32  003    France Bleu
            //   33  013    RFM
            //   34  016    RMC
            //   35  015    RTL
            //   36  017    RTL2
            //   37  018    Skyrock
            //   39  014    Rire et Chansons
            //   47  201    NRJ Global ProximitÃ© Premium
            //   50  303    Nova and Friends
            //   53  305    LagardÃ¨re PublicitÃ© News IDF
            //   54  020    Radio Nova
            //   55  306    Lip !
            //   56  307    Paris - IDF +
            //   59  019    Radio Classique
            //   60  310    Les IndÃ©s Capitale
            //   61  311    TF1 Pub Radios
            //   64  021    TSF JAZZ

            // Attention Total radio est obligatoirement a le fin avec total TV si present
            //   01  022    Total Radio



            //       DATA ISTA / 12,16,17,18,19,20,21,22,23,24,25,26,27,28,29,
            //-30,31,32,14,0,8,55,15,59,54,33,41,37,38 /

            //ISTA = new int[]
            //{
            //    0,12, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 14, 0, 8, 55, 15, 59, 54, 33,41, 37, 38
            //};

            //
            // INITIALISATIONS
            //
            int IG = 0;

            //
            //                              OUVERTURE FICHIER
            //
            FileStream fs = File.Open(PathGRPWave + @"\U120.SUP", FileMode.Open);
            fs.Seek(0, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fs);

            COMPT = 0;
            IG = 0;

            while (fs.Position != fs.Length)
            {
                // BOUCLE INDIVIDUS


                for (int i = 1; i <= NbGRPModulation; i++)
                    KHI2[i] = br.ReadUInt16();

                for (int i = 1; i <= 96; i++)
                    for (int j = 1; j <= NbGRPStation; j++)
                        ZLEC[i, j] = br.ReadByte();

                int IPERS = KHI2[1];
                if (IPERS < 0) IPERS = 32767;
                if (IPERS <= 0)
                    continue; //GOTO 30;

                IG = IG + 1;
                Console.WriteLine(IG);

                int IU = 1;
                if (KHI2[2] == 6) IU = 2;
                if (KHI2[2] == 7) IU = 3;

                // DETERMINATION DU SEGMENT
                ICSP = 1;
                if (KHI2[7] == 1 || KHI2[7] > 4) ICSP = 2;
                if (KHI2[7] > 6) ICSP = 3;
                ISEX = KHI2[3];
                if (ISEX > 2) ISEX = 2;
                IAGE = 1;
                if ((KHI2[22] >= 5) && (KHI2[22] <= 9)) IAGE = 2;
                if ((KHI2[22] == 10) || (KHI2[22] == 11)) IAGE = 3;
                IREG = 1;
                if (KHI2[15] > 6) IREG = 2;

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
                    int ISE2 = 16 + SEG1[ISEG];
                    int ISE3 = 26 + SEG2[SEG1[ISEG]];
                    int ISE4 = 32 + SEG3[SEG2[SEG1[ISEG]]];
                    IPOP[ISEG, IU] = IPOP[ISEG, IU] + IPERS;
                    IPOP[ISE2, IU] = IPOP[ISE2, IU] + IPERS;
                    IPOP[ISE3, IU] = IPOP[ISE3, IU] + IPERS;
                    IPOP[ISE4, IU] = IPOP[ISE4, IU] + IPERS;
                    IPOP[37, IU] = IPOP[37, IU] + IPERS;

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
                                    COUV[ISEG, IU, IS, IQ] = COUV[ISEG, IU, IS, IQ] + IPERS * ZLEC[IQ, NS];

                                    COUV[ISE2, IU, IS, IQ] = COUV[ISE2, IU, IS, IQ] + IPERS * ZLEC[IQ, NS];

                                    COUV[ISE3, IU, IS, IQ] = COUV[ISE3, IU, IS, IQ] + IPERS * ZLEC[IQ, NS];

                                    COUV[ISE4, IU, IS, IQ] = COUV[ISE4, IU, IS, IQ] + IPERS * ZLEC[IQ, NS];

                                    COUV[37, IU, IS, IQ] = COUV[37, IU, IS, IQ] + IPERS * ZLEC[IQ, NS];

                                }
                            }
                        }
                    }
                }

                if (ISEG == 2) COMPT = COMPT + 1;
            }

            for (int I = 1; I <= 37; I++)
            {
                for (int J = 1; J <= 3; J++)
                {
                    for (int K = 1; K <= NbStation; K++)
                    {
                        for (int L = 1; L <= 96; L++)
                        {
                            COUV[I, J, K, L] = COUV[I, J, K, L] / 12 / IPOP[I, J];
                        }
                    }

                }
            }

            return COUV;
        }

        private void cont75br(int NBIND, int NBSTA, int popLV, int popS, int popD, double[,,,] Couverture, string pathCouv)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CONTROLE DES GRP 75000 POUR CALAGE
            //
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            int[] POP = new int[3 + 1];
            double[] VAL = new double[3 + 1];
            int[] IVAL = new int[3 + 1];

            StringBuilder sb = new StringBuilder();
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                sb.AppendLine($"----- STATION {IP} -----");
                for (int I = 1; I <= 96; I++)
                {
                    sb.Append($"{I.ToString("0000")} ");
                    for (int J = 1; J <= 3; J++)
                    {
                        VAL[J] = Couverture[37, J, IP, I] * POP[J];
                        IVAL[J] = Convert.ToInt32((VAL[J] + 500) / 1000);
                        sb.Append($"{IVAL[J].ToString("000.000000000")} ");
                    }
                    sb.AppendLine();
                }
            }
            File.AppendAllText(pathCouv, sb.ToString());
        }

        // RESUL = ZUPTAUSE
        private double[,,,] cnzuptse(int NBIND, int NBSTA, string pathCnzuptse, double[,,,] COUV, int[,,,] REGRS, double[,,,] RESUL)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALAGE SUR GRP 75000 + ET RECALCUL DES PARAMETRES STATION
            //
            //        LUNDI - VENDREDI
            //
            int IDSUDRAD = 19;

            double[,,,] RESULCOR = new double[NBSTA + 1, 96 + 1, 4 + 1, 16 + 1];
            int[] TREG = new int[16 + 1];

            // OPEN(15, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\NIVEAUX',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // READ(15)REGRS
            // CLOSE(15)
            // OPEN(15, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\NOUVOGRP.SEM',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // READ(15)COUV
            // CLOSE(15)
            // OPEN(16, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\SORTIESE.COR',
            //-RECORDTYPE = 'TEXT')
            // OPEN(17, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\ZUPTAUSE',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // OPEN(18, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\ZUPTAUSE.COR',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')

            // BOUCLE STATIONS
            StringBuilder sortie = new StringBuilder();
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                sortie.AppendLine(IP.ToString());
                Console.WriteLine($"--------STATION {IP} --------");

                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    int NQ = IQ + 76;
                    if (NQ > 96) NQ = NQ - 96;
                    sortie.AppendLine(IP.ToString());
                    Console.WriteLine($"Traitement 1/4h {IQ}");

                    // AUCUNE AUDIENCE POUR CE 1 / 4h
                    for (int I = 1; I <= 16; I++)
                    {
                        TREG[I] = REGRS[1, IP, IQ, I];
                    }

                    if (TREG[1] != 5)
                    {

                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            double ZR = 0d;
                            double UR = 0d;
                            double PR = 0d;
                            double TAU = 0d;
                            int N2 = SEG1[N1];
                            int N3 = SEG2[N2];
                            int N4 = SEG3[N3];
                            int IN = TREG[N1] + 1;
                            if (((N1 == 1) || (IN == 1))
                                ||
                               (!((IN == 2 && SEG1[N1] == SEG1[N1 - 1]) ||
                                  (IN == 3 && SEG2[SEG1[N1]] == SEG2[SEG1[N1 - 1]]) ||
                                  (IN == 4 && SEG3[SEG2[SEG1[N1]]] == SEG3[SEG2[SEG1[N1 - 1]]]) ||
                                  (IN == 5))))
                            {

                                // LECTURE DES Z, U, P, TAU ORIGINAUX
                                //   20
                                ZR = RESUL[IP, IQ, 1, N1];
                                UR = RESUL[IP, IQ, 2, N1];
                                PR = RESUL[IP, IQ, 3, N1];
                                TAU = RESUL[IP, IQ, 4, N1];
                                double VA = 1d - ZR - UR;
                                double GRPA = VA * PR + UR;

                                if (IP != IDSUDRAD)
                                {
                                    int ISEG = N1;
                                    if (IN == 2) ISEG = 16 + SEG1[N1];
                                    if (IN == 3) ISEG = 26 + SEG2[SEG1[N1]];
                                    if (IN == 4) ISEG = 32 + SEG3[SEG2[SEG1[N1]]];
                                    if (IN == 5) ISEG = 37;
                                    // CALCUL NOUVEAU P
                                    double GRPN = COUV[ISEG, 1, IP, NQ];
                                    bool process = false;
                                    if (GRPA != 0d && GRPN != 0d)
                                    {
                                        double V = 1d - ZR - UR;
                                        if (V <= 0d)
                                        {
                                            UR = GRPN;
                                            ZR = 1d - UR;
                                            process = true;
                                        }
                                        else if (GRPN > 1d - ZR)
                                        {
                                            ZR = 1d - GRPN;
                                            V = 1d - ZR - UR;
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
                                                V = 1d - ZR - UR;
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
                            sortie.Append("TAU = " + TAU.ToString());

                        } // for (int N1 = 1; N1 <= 16; N1++)

                    }
                    else
                    {
                        sortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            RESULCOR[IP, IQ, 1, N1] = 1d;
                            RESULCOR[IP, IQ, 2, N1] = 0d;
                            RESULCOR[IP, IQ, 3, N1] = 0d;
                            RESULCOR[IP, IQ, 4, N1] = 0d;
                        }
                    }// if (TREG[1] != 5)
                } // for (int IQ = 1; IQ <= 96; IQ++)
            } // for (int IP = 1; IP <= NBSTA; IP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");
            // FORMAT('CELLULE ', I2, ' Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7, ' GRP=', F9.7, ' GRP''=', F9.7)

            if (File.Exists(pathCnzuptse)) File.Delete(pathCnzuptse);
            File.AppendAllText(pathCnzuptse, sortie.ToString());

            return RESULCOR;
        }

        private double[,,,] cnzuptsa(int NBIND, int NBSTA, string pathCnzuptsa, double[,,,] COUV, int[,,,] REGRS, double[,,,] RESUL)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALAGE SUR GRP 75000 + ET RECALCUL DES PARAMETRES STATION
            //
            //        SAMEDI
            //
            int IDSUDRAD = 19;

            double[,,,] RESULCOR = new double[NBSTA + 1, 96 + 1, 4 + 1, 16 + 1];
            int[] TREG = new int[16 + 1];

            // OPEN(15, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\NIVEAUX',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // READ(15)REGRS
            // CLOSE(15)
            // OPEN(15, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\NOUVOGRP.SEM',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // READ(15)COUV
            // CLOSE(15)
            // OPEN(16, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\SORTIESE.COR',
            //-RECORDTYPE = 'TEXT')
            // OPEN(17, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\ZUPTAUSE',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // OPEN(18, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\ZUPTAUSE.COR',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')

            // BOUCLE STATIONS
            StringBuilder sortie = new StringBuilder();
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                sortie.AppendLine(IP.ToString());
                Console.WriteLine($"--------STATION {IP} --------");

                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    int NQ = IQ + 76;
                    if (NQ > 96) NQ = NQ - 96;
                    sortie.AppendLine(IP.ToString());
                    Console.WriteLine($"Traitement 1/4h {IQ}");

                    // AUCUNE AUDIENCE POUR CE 1 / 4h
                    for (int I = 1; I <= 16; I++)
                    {
                        TREG[I] = REGRS[3, IP, IQ, I];
                    }

                    if (TREG[1] != 5)
                    {

                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            double ZR = 0d;
                            double UR = 0d;
                            double PR = 0d;
                            double TAU = 0d;
                            int N2 = SEG1[N1];
                            int N3 = SEG2[N2];
                            int N4 = SEG3[N3];
                            int IN = TREG[N1] + 1;
                            if (((N1 == 1) || (IN == 1))
                                ||
                               (!((IN == 2 && SEG1[N1] == SEG1[N1 - 1]) ||
                                  (IN == 3 && SEG2[SEG1[N1]] == SEG2[SEG1[N1 - 1]]) ||
                                  (IN == 4 && SEG3[SEG2[SEG1[N1]]] == SEG3[SEG2[SEG1[N1 - 1]]]) ||
                                  (IN == 5))))
                            {

                                // LECTURE DES Z, U, P, TAU ORIGINAUX
                                //   20
                                ZR = RESUL[IP, IQ, 1, N1];
                                UR = RESUL[IP, IQ, 2, N1];
                                PR = RESUL[IP, IQ, 3, N1];
                                TAU = RESUL[IP, IQ, 4, N1];
                                double GRPA = (1d - ZR - UR) * PR + UR;

                                if (IP != IDSUDRAD)
                                {
                                    int ISEG = N1;
                                    if (IN == 2) ISEG = 16 + SEG1[N1];
                                    if (IN == 3) ISEG = 26 + SEG2[SEG1[N1]];
                                    if (IN == 4) ISEG = 32 + SEG3[SEG2[SEG1[N1]]];
                                    if (IN == 5) ISEG = 37;
                                    // CALCUL NOUVEAU P
                                    double GRPN = COUV[ISEG, 3, IP, NQ];
                                    bool process = false;
                                    if (GRPA != 0d && GRPN != 0d)
                                    {
                                        double V = 1d - ZR - UR;
                                        if (V <= 0d)
                                        {
                                            UR = GRPN;
                                            ZR = 1d - UR;
                                            process = true;
                                        }
                                        else if (GRPN > 1d - ZR)
                                        {
                                            ZR = 1d - GRPN;
                                            V = 1d - ZR - UR;
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
                                                V = 1d - ZR - UR;
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
                            sortie.Append("TAU = " + TAU.ToString());

                        } // for (int N1 = 1; N1 <= 16; N1++)

                    }
                    else
                    {
                        sortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            RESULCOR[IP, IQ, 1, N1] = 1d;
                            RESULCOR[IP, IQ, 2, N1] = 0d;
                            RESULCOR[IP, IQ, 3, N1] = 0d;
                            RESULCOR[IP, IQ, 4, N1] = 0d;
                        }
                    }// if (TREG[1] != 5)
                } // for (int IQ = 1; IQ <= 96; IQ++)
            } // for (int IP = 1; IP <= NBSTA; IP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");
            // FORMAT('CELLULE ', I2, ' Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7, ' GRP=', F9.7, ' GRP''=', F9.7)

            if (File.Exists(pathCnzuptsa)) File.Delete(pathCnzuptsa);
            File.AppendAllText(pathCnzuptsa, sortie.ToString());

            return RESULCOR;
        }

        private double[,,,] cnzuptdi(int NBIND, int NBSTA, string pathCnzuptdi, double[,,,] COUV, int[,,,] REGRS, double[,,,] RESUL)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALAGE SUR GRP 75000 + ET RECALCUL DES PARAMETRES STATION
            //
            //        DIMANCHE
            //
            int IDSUDRAD = 19;

            double[,,,] RESULCOR = new double[NBSTA + 1, 96 + 1, 4 + 1, 16 + 1];
            int[] TREG = new int[16 + 1];

            // OPEN(15, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\NIVEAUX',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // READ(15)REGRS
            // CLOSE(15)
            // OPEN(15, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\NOUVOGRP.SEM',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // READ(15)COUV
            // CLOSE(15)
            // OPEN(16, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\SORTIESE.COR',
            //-RECORDTYPE = 'TEXT')
            // OPEN(17, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\ZUPTAUSE',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
            // OPEN(18, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\ZUPTAUSE.COR',
            //-RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')

            // BOUCLE STATIONS
            StringBuilder sortie = new StringBuilder();
            for (int IP = 1; IP <= NBSTA; IP++)
            {
                sortie.AppendLine(IP.ToString());
                Console.WriteLine($"--------STATION {IP} --------");

                // BOUCLE 1 / 4h
                for (int IQ = 1; IQ <= 96; IQ++)
                {
                    int NQ = IQ + 76;
                    if (NQ > 96) NQ = NQ - 96;
                    sortie.AppendLine(IP.ToString());
                    Console.WriteLine($"Traitement 1/4h {IQ}");

                    // AUCUNE AUDIENCE POUR CE 1 / 4h
                    for (int I = 1; I <= 16; I++)
                    {
                        TREG[I] = REGRS[2, IP, IQ, I];
                    }

                    if (TREG[1] != 5)
                    {

                        // BOUCLE CELLULES
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            double ZR = 0d;
                            double UR = 0d;
                            double PR = 0d;
                            double TAU = 0d;
                            int N2 = SEG1[N1];
                            int N3 = SEG2[N2];
                            int N4 = SEG3[N3];
                            int IN = TREG[N1] + 1;
                            if (((N1 == 1) || (IN == 1))
                                ||
                               (!((IN == 2 && SEG1[N1] == SEG1[N1 - 1]) ||
                                  (IN == 3 && SEG2[SEG1[N1]] == SEG2[SEG1[N1 - 1]]) ||
                                  (IN == 4 && SEG3[SEG2[SEG1[N1]]] == SEG3[SEG2[SEG1[N1 - 1]]]) ||
                                  (IN == 5))))
                            {

                                // LECTURE DES Z, U, P, TAU ORIGINAUX
                                //   20
                                ZR = RESUL[IP, IQ, 1, N1];
                                UR = RESUL[IP, IQ, 2, N1];
                                PR = RESUL[IP, IQ, 3, N1];
                                TAU = RESUL[IP, IQ, 4, N1];
                                double GRPA = (1d - ZR - UR) * PR + UR;

                                if (IP != IDSUDRAD)
                                {
                                    int ISEG = N1;
                                    if (IN == 2) ISEG = 16 + SEG1[N1];
                                    if (IN == 3) ISEG = 26 + SEG2[SEG1[N1]];
                                    if (IN == 4) ISEG = 32 + SEG3[SEG2[SEG1[N1]]];
                                    if (IN == 5) ISEG = 37;
                                    // CALCUL NOUVEAU P
                                    double GRPN = COUV[ISEG, 2, IP, NQ];
                                    bool process = false;
                                    if (GRPA != 0d && GRPN != 0d)
                                    {
                                        double V = 1d - ZR - UR;
                                        if (V <= 0d)
                                        {
                                            UR = GRPN;
                                            ZR = 1d - UR;
                                            process = true;
                                        }
                                        else if (GRPN > 1d - ZR)
                                        {
                                            ZR = 1d - GRPN;
                                            V = 1d - ZR - UR;
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
                                                V = 1d - ZR - UR;
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
                            sortie.Append("TAU = " + TAU.ToString());

                        } // for (int N1 = 1; N1 <= 16; N1++)

                    }
                    else
                    {
                        sortie.Append(" ***PAS D AUDIENCE POUR CE 1 / 4h * **");
                        for (int N1 = 1; N1 <= 16; N1++)
                        {
                            RESULCOR[IP, IQ, 1, N1] = 1d;
                            RESULCOR[IP, IQ, 2, N1] = 0d;
                            RESULCOR[IP, IQ, 3, N1] = 0d;
                            RESULCOR[IP, IQ, 4, N1] = 0d;
                        }
                    }// if (TREG[1] != 5)
                } // for (int IQ = 1; IQ <= 96; IQ++)
            } // for (int IP = 1; IP <= NBSTA; IP++)

            Console.WriteLine($"TRAITEMENT 1/4h ----");
            Console.WriteLine($"------- STATION ----");
            // FORMAT('CELLULE ', I2, ' Z=', F9.7, ' U=', F9.7, ' P=', F9.7, ' TAU=', F9.7, ' GRP=', F9.7, ' GRP''=', F9.7)

            if (File.Exists(pathCnzuptdi)) File.Delete(pathCnzuptdi);
            File.AppendAllText(pathCnzuptdi, sortie.ToString());

            return RESULCOR;
        }


        private bool L1BITFCT(int[] ZLEC, int ind)
        {
            int sizeBit = (sizeof(int) * 8);
            int index = 1 + (ind - 1) / sizeBit;
            int pos = ind % sizeBit;
            return BTEST(ZLEC[index], pos);
        }

        private bool BTEST(int value, int pos)
        {
            return (value & (1 << pos)) != 0;
        }


        // SUBROUTINE MINIMISE
        // optimisation de la courbe theorique de montee en audience
        // suivant la beta binomiale
        private void MINIMISE(double GRP, double[] ZC, ref double PR, ref double UR, ref double ZR, out double TAU, double ZA, double UA, double NB, bool qhps = false)
        {
            double Z, Z1, Z2, DELTA0, DELTZ0, DELTZ1, DELTZ2, DELTA00, DELTATO;
            double[] RES = new double[5] { 0, ZA, UA, 0d, 0d };
            double DZ0, DT0, DZ, DU;
            double V0 = 1d - ZA - UR;
            if (V0 > 0.00005)
            {
                DELTA0 = 0d;
                for (int I = 1; I <= NB; I++)
                {
                    DELTA0 = DELTA0 + 2 * I * I * ZC[I] * ZC[I];
                }
                DELTA00 = DELTA0;
                DELTATO = DELTA0;
                DELTZ0 = DELTA0;
                DELTZ1 = DELTA0;
                DELTZ2 = DELTA0;
                DZ0 = DT0 = DZ = DU = 0.1;

                Z = ZA + (1d - ZA) / 2;
                if (!qhps || Z >= ZR)
                {
                    if ((1d - Z) < DZ) DZ = 1d - Z - 0.00001;
                    Z1 = Z;
                    Z2 = ZR;
                    double U0 = UR;
                    //5 
                    bool sortie = false;
                    for (; ; )
                    {
                        double ITZ = 0;
                        if (Z >= ZR)
                        {
                            if (DZ > (Z1 - Z2)) DZ = Z1 - Z2;
                            // VARIATION DES JAMAIS
                            //10 
                            for (; ; )
                            {
                                ITZ = ITZ + 1;

                                MINIMU(ITZ, Z, GRP, ZC, ZA, UA, U0, NB, DELTA00, out double DELTZ);

                                if (DELTZ <= DELTZ0)
                                {
                                    if (!qhps || (DELTZ != DELTZ0 || DELTZ0 != DELTZ1 || DELTZ1 != DELTZ2))
                                    {
                                        // LA DISTANCE A DIMINUE
                                        DELTZ2 = DELTZ1;
                                        DELTZ1 = DELTZ0;
                                        DELTZ0 = DELTZ;
                                        if (Z > Z2)
                                        {
                                            Z = Z - DZ;
                                            continue; // GOTO 10
                                        }
                                        if (DZ <= 0.0001)
                                        {
                                            sortie = true;
                                            break;
                                        }

                                        Z2 = Z;
                                        Z = Z + DZ * 2;
                                        DZ = DZ / 10d;
                                        continue; // GOTO 10
                                    }
                                    else
                                    {
                                        sortie = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    // LA DISTANCE A AUGMENTE
                                    if (ITZ > 2)
                                    {
                                        if (DZ <= 0.0001)
                                        {
                                            sortie = true;
                                            break;
                                        }
                                        Z2 = Z;
                                        Z = Z + 2 * DZ;
                                        DZ = DZ / 10d;
                                        DELTZ0 = DELTZ2;
                                        break; // GOTO 5
                                    }
                                    if (DZ <= 0.0001)
                                    {
                                        sortie = true;
                                        break;
                                    }
                                    Z2 = Z;
                                    Z = Z1;
                                    DZ = DZ / 10d;
                                    DELTZ0 = DELTZ1;
                                    break; // GOTO 5
                                }
                            } // for (; ; )  du 10
                        }// if (Z >= ZR)
                        if (sortie) break; // GOTO 5 suite...
                    } // for (; ; )  du 5
                }
                ZR = RES[1];
                UR = RES[2];
                PR = RES[3];
                TAU = RES[4];
                return;
            } // if (V0 > 0.00005)

            ZR = ZA;
            TAU = 0d;
        }

        // SUBROUTINE MINITAU
        // CALCUL DE LA VALEUR OPTIMALE DE TAU
        private void MINITAU(double[] ZC, double[] X, double ZA, double NB, double U0, double Q, double Z, double U, double P, double V, out double DELTT, ref double DELTA00)
        {
            double[] Y = new double[15];
            double[] RES = new double[5];
            double T, DELTA0, T1, T2, DMOINS1 = 0, DMOINS2 = 0, DELTATO = 0, DT;

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
                    DELTA = DELTA + K * Math.Pow(ZC[K] * 0.01 - Y[K], 2d);
                }
                DELTA = DELTA + 0.1 * Math.Pow(ZA - Z, 2) / ZA;

                if (U0 > 0) DELTA = DELTA + 0.1 * Math.Pow(U0 - U, 2) / U0;

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

                    if (DT != 0.1) break;
                }
                else
                {
                    // LA DISTANCE A AUGMENTE
                    if (ITE > 2)
                    {
                        if (DT > 0.0001)
                        {
                            DELTA0 = DMOINS2;
                            T1 = T - 2 * DT;
                            T2 = T;
                            DT = DT / 10d;
                            ITE = 0;
                            T = T1;
                            continue;
                        }

                        if (DT > 0.0001)
                        {
                            T2 = T;
                            DT = DT / 10d;
                            ITE = 0;
                            T = T1;
                            continue;
                        }
                    }

                    DELTT = DELTA0;
                    break;
                }

                T2 = T2 * 2;
                T1 = T1 * 2;

                if (T2 < 16)
                {
                    ITE = 0;
                    T = T1;
                }
            }
            Console.WriteLine($" U= {U}  Z= {Z}  T= {T}  P= {P} ");
        }

        // SUBROUTINE MINIMU
        // CALCUL DU U OPTIMAL
        // EN FONCTION DE Z
        private void MINIMU(double ITZ, double Z, double GRP, double[] ZC, double ZA, double UA, double U0, double NB, double DELTA00, out double DELTZ)
        {
            double[] X = new double[15 + 1];
            double P, U, V, Q, U1, U2, DELTU, DELTU0, DELTU1, DELTU2;

            DELTZ = 0;
            DELTU0 = DELTA00;
            DELTU1 = DELTU0;
            DELTU2 = DELTU0;
            DELTU = 0;

            // INCREMENTS INITIAUX
            double DU = 0.01;
            U1 = 0d;
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
                    ITU++; ;
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
                                DU = U - 1 - Z;
                                U = 1d - Z - 0.00001;
                            }
                            continue;
                        }
                        if (DU <= 0.00001)
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
                        if (DU > 0.00001)
                        {
                            DELTZ = DELTU0;
                            return;
                        }
                        if (ITU >= 2) U1 = U - 2 * DU;
                        if (ITU < 2)
                        {
                            U1 = U - DU;
                            if (U >= (1d - Z)) DU = U - 1 - Z;
                            if (U >= (1d - Z)) U = 1d - Z - 0.00001;
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
                    DU = DU / 10;
                    U = U1;
                    ITU = 0;
                    continue;
                }
                break;
            } // for (; ; )
        }
    }

    public struct Fushab09Indiv
    {
        public int[] AVANT;
        public int[,] KHAB;
        public int[,] KHSA;
        public int[,] KHDI;
        public int[] APRES;
        public int[] CHARIOT;

        public Fushab09Indiv(int SIGN_LINE_LEN_BEFORE_HAB, int NB_STA_ALL_HAB)
        {
            AVANT = new int[SIGN_LINE_LEN_BEFORE_HAB + 1];
            KHAB = new int[9 + 1, NB_STA_ALL_HAB + 1];
            KHSA = new int[9 + 1, NB_STA_ALL_HAB + 1];
            KHDI = new int[9 + 1, NB_STA_ALL_HAB + 1];
            APRES = new int[SIGN_LINE_LEN_BEFORE_HAB + 1];
            CHARIOT = new int[2];
        }
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

    public struct VsorPoid
    {
        public int[,] VSor;
        public int[] Poid;
        public VsorPoid(int NBSTA)
        {
            VSor = new int[6 + 1, NBSTA];
            Poid = new int[96 + 1];
        }
    }
    #endregion Structures
}

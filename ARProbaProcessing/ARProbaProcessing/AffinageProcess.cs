using System;
using System.Collections.Generic;
using System.IO;

namespace ARProbaProcessing
{
    public class AffinageProcess
    {
        private int[] NOTE = new int[25 + 1] { 999999, 0, 12, 6, 4, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        private int[] NBIT = new int[] { 999999, 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

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

            segpanel();

            VsorPoid[][] JN = ecrpan1j(pathF04, NbStation, nbStationTotal, ITS, year);   // [Jour 1..23][Individus 1..N] = {VOSR[,]?, Poid[]}

            VsorPoid[][][] JNByWeek = regr5jp2(JN);

            List<Fushab09Indiv> fushab09Indivs = Fushab09(NbStation, SIGN_LINE_LEN_BEFORE_HAB, NB_STA_ALL_HAB, TABRH);
            int NBINDIV = fushab09Indivs.Count;

            int[,,] NINI_IND_QH_W = chab1qhp(NbStation, fushab09Indivs); // Habitude

            int[,] NINI_IND_STA = crenonin(nbJour, NbStation, fushab09Indivs, JN);

            ecrsegpa(pathSig, COL_PIAB, COL_CSCI, COL_SEX, COL_AGE, COL_RUDA, NbStation, NBINDIV, SIGN_LINE_LEN_FULL, 
                     out int[,] POIDSEGM, out int IPOP);

            int[,,,] cellules = calcregr(NBINDIV, NB_STA_HAB_NOTO, NINI_IND_STA, POIDSEGM, JN);

            calcnivo();
            caud1qhp();

            caudtotp();

            sav1qhpa();
            sav1qhps();
            sav1qhpd();

            int[,,,] Couverture = cgrp75br(PathGRPWave, NbStation, NbGRPModulation, NbGRPStation, NotorieteStation);
            cont75br();

            cnzuptse();
            cnzuptsa();
            cnzuptdi();

            attribp2();

            transp08();
            crecib08();
            penetr();
            asympt();
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

        private void segpanel()
        {

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

            // C Attention AVANT(#SIGN_LINE_LEN_BEFORE_HAB#) et le buffer de lecture pour se caler au début des données de HAB
            // C Attention APRES(#SIGN_LINE_LEN_AFTER_HAB#) et le buffer de lecture pour se caler à la fin de la ligne des données

            // INTEGER * 1 AVANT(#SIGN_LINE_LEN_BEFORE_HAB#),KHAB(9,#NB_STA_ALL_HAB#),KHSA(9,#NB_STA_ALL_HAB#),KHDI(9,#NB_STA_ALL_HAB#),APRES(#SIGN_LINE_LEN_AFTER_HAB#),CHARIOT(2)
            // INTEGER * 1 KHA2(9, NBSTA),KHS2(9, NBSTA),KHD2(9, NBSTA)
            int[,] KHA2 = new int[9 + 1, NBSTA + 1];
            int[,] KHS2 = new int[9 + 1, NBSTA + 1];
            int[,] KHD2 = new int[9 + 1, NBSTA + 1];

            // => EN PARAMETRE
            // INTEGER * 2 TABRH(NBSTA)
            // C Attention pour les stations avec Notoriété il faut metre les données total radio(#TOTAL_RADIO_INDEX#)
            // int[] TABRH = new int[NBSTA + 1]; 
            // int[] TABRH /#HAB_STA_LIST_ID_NOTO_SET_TO_TOTAL_RADIO#/

            // INTEGER*1 int KHAB(9,NB_STA_ALL_HAB),KHSA(9,#NB_STA_ALL_HAB#),KHDI(9,#NB_STA_ALL_HAB#),APRES(#SIGN_LINE_LEN_AFTER_HAB#), CHARIOT(2)
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

            // Attention AVANT(#SIGN_LINE_LEN_BEFORE_HAB#) et le buffer de lecture pour se caler au début des données de HAB
            // Attention APRES(#SIGN_LINE_LEN_AFTER_HAB#) et le buffer de lecture pour se caler à la fin de la ligne des données
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
            int[] KHI2 = new int[SIGN_LINE_LEN_FULL + 1];
            SEGM = new int[NBIND + 1, 5 + 1];
            int ICSP, IREG, ISEX, IAGE, ISEG;
            int[] SEG2 = new int[16 + 1] { 999999, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 9, 10, 10 };
            int[] SEG3 = new int[10 + 1] { 999999, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6 };
            int[] SEG4 = new int[6 + 1] { 999999, 1, 2, 2, 3, 3, 4 };
            int[] C = new int[17];
            IPOP = 0;

            // OUVERTURE FICHIER
            FileStream fs = File.Open(Path.Combine(pathSig, "sig20jfc.bde"), FileMode.Open);
            fs.Seek(0, SeekOrigin.Begin);
            BinaryReader br = new BinaryReader(fs);

            // BOUCLE INDIVIDUS
            int IG = 0;
            while (fs.Position != fs.Length)
            {
                for (int i = 1; i <= SIGN_LINE_LEN_FULL; i++)
                    KHI2[i] = br.ReadByte();

                ISEG = 0;
                // CALCUL DU POIDS(Colonnes 9 à 13 inclues)
                int IPERS = 10000 * (KHI2[COL_PIAB] - 48) + 1000 * (KHI2[COL_PIAB + 1] - 48) + 100 * (KHI2[COL_PIAB + 2] - 48) + 10 * (KHI2[COL_PIAB + 3] - 48) + (KHI2[COL_PIAB + 4] - 48);
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
                    if ((KHI2[COL_CSCI] - 48) == 1 || (KHI2[COL_CSCI] - 48) > 4) ICSP = 2;
                    if ((KHI2[COL_CSCI] - 48) > 6) ICSP = 3;

                    //--------------------
                    // Sexe de l'interviewé
                    //--------------------
                    // Le sexe de l'interviewé est codé sur une colonne (colonne #COL,SEXE#)
                    // ISEX = 1 correspond aux Hommes
                    // ISEX = 2 correspond aux Femmes
                    ISEX = KHI2[COL_SEX] - 48;

                    //------------------ -
                    // Age de l'interviewé
                    //------------------ -
                    // L'Age de l'interviewé est codé sur deux colonnes(colonne #COL,AG11,0# et #COL,AG11,1#)
                    // IAGE = 1 correspond aux 13 - 34 ans
                    // IAGE = 2 correspond aux 35 - 59 ans
                    // IAGE = 3 correspond aux 60 ans et +
                    int AGE = 10 * (KHI2[COL_AGE] - 48) + (KHI2[COL_AGE + 1] - 48);
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
                    if ((KHI2[COL_RUDA] - 48) > 6) IREG = 2;

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

        private void calcnivo()
        {

        }

        private void caud1qhp()
        {

        }

        private void caudtotp()
        {

        }

        private void sav1qhpa()
        {

        }
        private void sav1qhps()
        {

        }
        private void sav1qhpd()
        {

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
        private int[,,,] cgrp75br(string PathGRPWave, int NbStation, int NbGRPModulation, int NbGRPStation, int[] NotorieteStation)
        {
            if (NbStation == 0
                || NbGRPModulation == 0
                || NbGRPStation == 0
                || NotorieteStation == null)
                return null;


            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES GRP 75000 POUR CALAGE

            int[,,,] COUV = new int[37 + 1, 3 + 1, NbStation + 1, 96 + 1];
            int[] SEG1 = new int[16 + 1];
            int[] SEG2 = new int[10 + 1];
            int[] SEG3 = new int[6 + 1];
            int[,] IPOP = new int[37 + 1, 3 + 1];


            int[] KHI2 = new int[NbGRPModulation + 1];
            int[] ISTA = new int[NbStation + 1];
            int ICSP, IREG, ISEX, IAGE, ISEG, COMPT;
            int[,] ZLEC = new int[96 + 1, NbGRPStation + 1];


            SEG1 = new int[] { 0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 9, 9, 10, 10 };
            SEG2 = new int[] { 0, 1, 1, 2, 3, 3, 4, 4, 5, 6, 6 };
            SEG3 = new int[] { 0, 1, 2, 2, 3, 3, 4 };


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

                //
                int IPERS = KHI2[1];
                if (IPERS < 0) IPERS = 32767;
                if (IPERS <= 0)
                    continue; //GOTO 30;

                IG = IG + 1;
                //write(*, *) IG;
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


                //////////////////////////////
                if (IAGE == 3)
                {
                    // 70
                    ISEG = 13;
                    if (IREG == 2) ISEG = 14;
                    if (ISEX == 2) ISEG = ISEG + 2;
                }
                else if (ICSP != 1)
                {
                    // 50
                    if (ICSP == 3)
                    {
                        // 60
                        if (IAGE == 2)
                        {
                            // 65
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
                        // 55
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


                //////////////////////////////

                if (ISEG < 1 || ISEG > 16)
                {
                    //print *,' ISEG=',ISEG;

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

            //// FIN DE FICHIER
            //120 OPEN(16, FILE = 'C:\Affinage\PANEL_~1\Panfra20\Output\SORTIE8.TXT', RECORDTYPE = 'TEXT');
            //WRITE(16, 1) IG;


            for (int I = 1; I <= 37; I++)
            {
                //WRITE(16, 2) I,(IPOP[I, J], J = 1, 3);


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
        private void cont75br()
        {

        }

        private void cnzuptse()
        {

        }
        private void cnzuptsa()
        {

        }
        private void cnzuptdi()
        {

        }

        private void attribp2()
        {

        }

        private void transp08()
        {

        }
        private void crecib08()
        {

        }
        private void penetr()
        {

        }
        private void asympt()
        {

        }
        
        private bool L1BITFCT(int[] ZLEC, int ind)
        {
            int sizeBit = (sizeof(int) * 8);
            int index = 1 + (ind-1) / sizeBit;
            int pos = ind % sizeBit;
            return BTEST(ZLEC[index], pos);
        }

        private bool BTEST(int value, int pos)
        {
            return (value & (1 << pos)) != 0;
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

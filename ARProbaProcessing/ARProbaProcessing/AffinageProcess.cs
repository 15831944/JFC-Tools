using System;
using System.Collections.Generic;
using System.IO;

namespace ARProbaProcessing
{
    public class AffinageProcess
    {
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

            lecpanel(0, "", 0, 0, 0, 0, 0, 0, 0, out lstPoids, out lstAges, out lstFiltreIDF);
            #region entrÈes Fushab09
            int SIGN_LINE_LEN_BEFORE_HAB = 0;
            int NB_STA_ALL_HAB = 0;
            int[] TABRH = null;
            #endregion

            segpanel();
            ecrpan1j();
            regr5jp2(0);

            List<Fushab09Indiv> Fushab09Indivs = Fushab09(NbStation, SIGN_LINE_LEN_BEFORE_HAB, NB_STA_ALL_HAB, TABRH);

            chab1qhp();
            crenonin();

            ecrsegpa();
            CALCREGR();
            calcnivo();
            caud1qhp();

            caudtotp();

            sav1qhpa();
            sav1qhps();
            sav1qhpd();

            int[, , ,] Couverture = cgrp75br(PathGRPWave, NbStation, NbGRPModulation, NbGRPStation, NotorieteStation);
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
            // Attention, le nombre de caract√®res r√©el est de 599 colonnes(on en rajoute 2 pour le retour Chariot)
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

                // Filtre REGION PARISIENNE(R√©gion Uda)

                ICL = 0;

                if ((KHI2[COL_RUDA] - 48) == 1)
                {
                    ICL = 1;
                    COMPTIDF = COMPTIDF + 1;
                }



                // CALCUL DU POIDS(Colonnes 9 √† 13 inclues)

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
        private void ecrpan1j()
        {

        }
        private void regr5jp2(int LENENR)
        {

//            int[,] KHI3 = new int[LENENR, 5];   // INTEGER *2 KHI3(LENENR,5)
//            string C2, C1, C3;                  // CHARACTER C2*2, C1*1,C3*1
      
//            // INITIALISATIONS
//            // OUVERTURE FICHIERS
//            for (int J=1; J<=3; J++)
//            { 
//                int IG=0
//                C1 = J

//                OPEN(14,FILE='#OUTPUT#jfc\PANSEM'//C1//'.JFC',  
//                -FORM='UNFORMATTED',RECORDTYPE='FIXED')
       
//        DO I = 3+(J-1)*7,7+(J-1)*7
//          IO=20+I
//          WRITE(C2,'(I2.2)') I
//          OPEN(IO,FILE='#OUTPUT#jfc\JN'//C2//'NIV.JFC',
//     -FORM='UNFORMATTED',RECORDTYPE='FIXED')
       
//        ENDDO	
//C                              BOUCLE INDIVIDUS
//C
//   30   IG=IG+1
//        DO I = 3+(J-1)*7,7+(J-1)*7
//          IJ=I-(J-1)*7-2
//          IO=20+I
//          READ(IO) (KHI3(K,IJ),K=1,LENENR)
//        ENDDO
//        WRITE(14)KHI3
//        IF(IG.LT.NBIND) GO TO 30
//C                              FIN DE FICHIER
//        WRITE(C3,'(I1.1)') J+1
//  120   OPEN(16,FILE='#OUTPUT#SORTIE'//C3//'.txt',RECORDTYPE='TEXT')
//        WRITE(16,1) IG
//        CLOSE(16)
//        DO I = 3+(J-1)*7,7+(J-1)*7
//          IO=20+I
//          CLOSE(IO)
//        ENDDO
//        CLOSE(14)
//      ENDDO
//C           
//      STOP
//    1 FORMAT('1   NBGUS:',I6)
//            CHARACTER C2*2, C1 * 1,C3 * 1
//C
//C                              INITIALISATIONS
//C
//C
//C                              OUVERTURE FICHIERS
//C
//      DO J = 1,3
//        IG = 0
//        WRITE(C1, '(I1.1)') J
//        OPEN(14, FILE = '#OUTPUT#jfc\PANSEM'//C1//'.JFC',
//     - FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')


//        DO I = 3 + (J - 1) * 7,7 + (J - 1) * 7
//          IO = 20 + I
//          WRITE(C2, '(I2.2)') I
//          OPEN(IO, FILE = '#OUTPUT#jfc\JN'//C2//'NIV.JFC',
//     - FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED')


//        ENDDO
//C                              BOUCLE INDIVIDUS
//C
//   30   IG = IG + 1
//        DO I = 3 + (J - 1) * 7,7 + (J - 1) * 7
//          IJ = I - (J - 1) * 7 - 2
//          IO = 20 + I
//          READ(IO)(KHI3(K, IJ), K = 1, LENENR)
//        ENDDO
//        WRITE(14)KHI3
//        IF(IG.LT.NBIND) GO TO 30
//C FIN DE FICHIER
//        WRITE(C3, '(I1.1)') J + 1
//  120   OPEN(16, FILE = '#OUTPUT#SORTIE'//C3//'.txt',RECORDTYPE='TEXT')
//        WRITE(16, 1) IG
//        CLOSE(16)
//        DO I = 3 + (J - 1) * 7, 7 + (J - 1) * 7
//          IO = 20 + I
//          CLOSE(IO)
//        ENDDO
//        CLOSE(14)
//      ENDDO
//C
//      STOP
//    1 FORMAT('1   NBGUS:', I6)
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

            // C Attention AVANT(#SIGN_LINE_LEN_BEFORE_HAB#) et le buffer de lecture pour se caler au dÈbut des donnÈes de HAB
            // C Attention APRES(#SIGN_LINE_LEN_AFTER_HAB#) et le buffer de lecture pour se caler ‡ la fin de la ligne des donnÈes

            // INTEGER * 1 AVANT(#SIGN_LINE_LEN_BEFORE_HAB#),KHAB(9,#NB_STA_ALL_HAB#),KHSA(9,#NB_STA_ALL_HAB#),KHDI(9,#NB_STA_ALL_HAB#),APRES(#SIGN_LINE_LEN_AFTER_HAB#),CHARIOT(2)
            // INTEGER * 1 KHA2(9, NBSTA),KHS2(9, NBSTA),KHD2(9, NBSTA)
            int[,] KHA2 = new int[9 + 1, NBSTA + 1];
            int[,] KHS2 = new int[9 + 1, NBSTA + 1];
            int[,] KHD2 = new int[9 + 1, NBSTA + 1];

            // => EN PARAMETRE
            // INTEGER * 2 TABRH(NBSTA)
            // C Attention pour les stations avec NotoriÈtÈ il faut metre les donnÈes total radio(#TOTAL_RADIO_INDEX#)
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
            while (fs.Position != fs.Length)
            {
                // BOUCLE INDIVIDUS
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

        private void chab1qhp()
        {

        }
        private void crenonin()
        {

        }

        private void ecrsegpa()
        {

        }
        private void CALCREGR()
        {

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
        /// <param name="PathGRPWave"> Chemin pour acc√®der au fichier de la 126000</param>
        /// <param name="NbStation"> Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)</param>
        /// <param name="NbGRPModulation">Nombre de modalit√©s dans le fichier de GRP 89(Attention + 2 pour le vecteur de poids et un vecteur vide dans le fichier U109)</param>
        /// <param name="NbGRPStation">Nombre de station dans la fichier de GRP U109</param>
        /// <param name="NotorieteStation"></param>
        /// <returns></returns>
        private int[, , ,] cgrp75br(string PathGRPWave, int NbStation, int NbGRPModulation, int NbGRPStation, int[] NotorieteStation)
        {
            if (NbStation == 0
                || NbGRPModulation == 0
                || NbGRPStation == 0
                || NotorieteStation == null)
                return null;


            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // CALCUL DES GRP 75000 POUR CALAGE

            int[, , ,] COUV = new int[37 + 1, 3 + 1, NbStation + 1, 96 + 1];
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
            //   16  101    Les Ind√©s Radios
            //   21  001    Ch√©rie FM
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
            //   47  201    NRJ Global Proximit√© Premium
            //   50  303    Nova and Friends
            //   53  305    Lagard√®re Publicit√© News IDF
            //   54  020    Radio Nova
            //   55  306    Lip !
            //   56  307    Paris - IDF +
            //   59  019    Radio Classique
            //   60  310    Les Ind√©s Capitale
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
}

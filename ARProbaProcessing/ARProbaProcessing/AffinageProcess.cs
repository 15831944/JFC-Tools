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

            lecpanel();
            segpanel();
            ecrpan1j();
            regr5jp2();
            fushab09();
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

        private void lecpanel()
        {

        }

        private void segpanel()
        {

        }
        private void ecrpan1j()
        {

        }
        private void regr5jp2()
        {

        }
        private void fushab09()
        {

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
        /// <param name="PathGRPWave"> Chemin pour accèder au fichier de la 126000</param>
        /// <param name="NbStation"> Le nombre de station correspond au nombre de stations(30) -1 pour Total Radio(et Total TV)</param>
        /// <param name="NbGRPModulation">Nombre de modalités dans le fichier de GRP 89(Attention + 2 pour le vecteur de poids et un vecteur vide dans le fichier U109)</param>
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
            //   16  101    Les Indés Radios
            //   21  001    Chérie FM
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
            //   47  201    NRJ Global Proximité Premium
            //   50  303    Nova and Friends
            //   53  305    Lagardère Publicité News IDF
            //   54  020    Radio Nova
            //   55  306    Lip !
            //   56  307    Paris - IDF +
            //   59  019    Radio Classique
            //   60  310    Les Indés Capitale
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
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ARProbaProcessing
{
    public partial class AffinageProcess
    {
        private byte[,,,] attribp2(int NBIND, int NBSTA, byte[,,,] REGRS, int[,] KHI2, int[,] NINI, float[] NOTES, byte[,,,] AUDI, int[,,,] HABI,
            float[,,,] ZUPTAUSECOR, float[,,,] ZUPTAUSACOR, float[,,,] ZUPTAUDICOR, string pathSortie, string pathPANECR)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // ATTRIBUTION DES PROBAS 1/4h PAR 1/4h,L-V,SAM,DIM
            //        STATION PAR STATION
            // Le nombre de station correspond au nombre de stations (#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)
            byte[,,,] PROBAS = new byte[NBSTA + 1, 3 + 1, 96 + 1, NBIND + 1];
            int NSEG = 100;
            int JMAX = NSEG * 2;
            int JSUP = JMAX + 2;

            double[] LOGS = new double[JMAX + 1];
            double[] DISTR = new double[NSEG + 1];
            double[] REP = new double[NSEG + 1];
            double[] VREP = new double[NSEG + 1];
            double[] GT = new double[16 + 1];

            double XJ, LAMBDA, DELTA, NG, GRPN, X, Y;
            double[] SCORE = new double[NBIND + 1];
            int[] TAB = new int[NBIND + 1];
            double NOTI, SGN, SGN0;
            int[] POIDS = new int[NBIND + 1];
            int[] IPPS = new int[5 + 1];
            byte PR;
            int R, RSAVE, RP, H, HCR;
            int[] NB = new int[5 + 1];
            int[] IM = new int[5 + 1];
            int[] RANK = new int[NBIND + 1 + 1];
            int[] RANG = new int[NBIND + 1];
            int[] CELL = new int[NBIND + 1];
            int[] PT1 = new int[16 + 1];
            int[] PTNSEG = new int[16 + 1];
            int[] TREG = new int[16 + 1];
            double GRP = 0f;
            int NPERR = 0;

            // INITIALISATIONS
            StringBuilder sbSortie = new StringBuilder();
            XJ = JSUP;
            LOGS[1] = 0;
            for (int J = 2; J <= JMAX; J++)
            {
                LOGS[J] = Math.Log(Convert.ToDouble(J) / XJ);
            }

            // OPEN(30, FILE= '#OUTPUT#PANECR#YEAR#',
            //-RECORDTYPE= 'FIXED', FORM= 'UNFORMATTED')

            for (int IG = 1; IG <= NBIND; IG++)
            {
                int IPERS = KHI2[IG, 1];
                IPERS *= 10;
                POIDS[IG] = IPERS;
            }

            FileStream writeStream = new FileStream(pathPANECR, FileMode.Create);
            BinaryWriter writeBinary = new BinaryWriter(writeStream);

            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
                Console.WriteLine($"Traitement station {NOP}");

                // BOUCLE UNIVERS
                for (int IU = 1; IU <= 3; IU++)
                {
                    float[,,,] RESUL = null;
                    // BOUCLE 1/4h
                    for (int IQ = 1; IQ <= 96; IQ++)
                    {

                        if (IU == 1) RESUL = ZUPTAUSECOR;
                        if (IU == 2) RESUL = ZUPTAUSACOR;
                        if (IU == 3) RESUL = ZUPTAUDICOR;
                        for (int I = 1; I <= NBIND; I++)
                        {
                            PROBAS[NOP,IU,IQ,I] = 0;
                        }

                        byte[] AUDI_ = new byte[NBIND+1];
                        int[] HABI_ = new int[NBIND + 1];
                        int[] NINI_ = new int[NBIND + 1];
                        for (int I = 1; I <= NBIND; I++)
                        {
                            AUDI_[I] = AUDI[NOP, I, IQ, IU];
                            HABI_[I] = HABI[NOP, I, IQ, IU];
                            NINI_[I] = NINI[I, NOP];
                        }

                        // TRI DES SCORES INDIVIDUELS
                        for (int I = 1; I <= NBIND; I++)
                        {
                            SCORE[I] = AUDI[NOP, I, IQ, IU];   // [STATIONS, INdiv, QH, 1..3]
                            if (SCORE[I] < 0) SCORE[I] += 256;
                            SCORE[I] = 10 * SCORE[I] + HABI[NOP, I, IQ, IU];

                            NOTI = NOTES[I];
                            if (NINI[I, NOP] == 1) NOTI = 0f;
                            SCORE[I] = 1000d * Convert.ToDouble( SCORE[I]) + NOTI;
                            RANG[I] = I;
                        }

                        SORTF(1, NBIND, RANG, SCORE);

                        for (int I = 1; I <= NBIND; I++)
                        {
                            for (int J = 1; J <= NBIND; J++)
                            {
                                if (I == RANG[J]) CELL[I] = J;
                            }
                        }

                        // AUCUNE AUDIENCE POUR CE 1/4h
                        int IP = NOP;
                        if (REGRS[IU, IP, IQ, 1] != 5)
                        {
                            GRP = 0;
                            NPERR = 0;
                            // BOUCLE CELLULES
                            for (int N1 = 1; N1 <= 16; N1++)
                            {
                                TREG[N1] = REGRS[IU, IP, IQ, N1];
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

                                    for (int J = 1; J <= 5; J++)
                                    {
                                        IM[J] = 0;
                                        IPPS[J] = 0;
                                    }
                                    // BOUCLE INDIVIDUS
                                    int II = 0;
                                    for (int IG = 1; IG <= NBIND; IG++)
                                    {
                                        if ((NIV==5 && NB[NIV]==1) || (KHI2[IG, NIV + 1] == NB[NIV]))
                                        {
                                            IM[NIV]++;
                                            IPPS[NIV] += POIDS[IG];
                                            PROBAS[NOP, IU, IQ, IG] = 255; // -1 ?
                                            II++;
                                            RANK[II] = IG;
                                            SCORE[II] = CELL[IG];
                                            RANG[II] = II;
                                        }
                                    }
                                    int NICEL = II;
                                    SORTF(1, NICEL, RANG, SCORE);
                                    for (int I = 1; I <= NICEL; I++)
                                    {
                                        TAB[I] = RANK[I];
                                    }
                                    for (int I = 1; I <= NICEL; I++)
                                    {
                                        RANK[I] = TAB[RANG[I]];
                                    }
                                    PT1[N1] = 0;
                                    PTNSEG[N1] = 0;
                                    // LECTURE DES PARAMETRES Z,U,P,TAU    [STATIONS, QH, DATAS ZR-UR-PR-TAUX, CELL]
                                    float ZR = RESUL[NOP, IQ, 1, N1];


                                    if (ZR >= 1d)
                                    {
                                        for (int I = 1; I <= NICEL; I++)
                                        {
                                            H = RANK[I];
                                        }
                                        GRP = 0;
                                        NPERR = 0;
                                        continue;
                                    }

                                    float UR = RESUL[NOP, IQ, 2, N1];
                                    float P = RESUL[NOP, IQ, 3, N1];
                                    float TAU = RESUL[NOP, IQ, 4, N1];
                                    GRPN = UR + P * (1f - ZR - UR);
                                    if (float.IsNaN(P)) P = 0f;
                                    CALDISTR(ZR, UR, P, TAU, LOGS, out DISTR, out REP);

                                    // IMPRESSION DES RESULTATS
                                    int IN = TREG[N1] + 1;

                                    // REPARTITION DANS LA CELLULE
                                    for (int I = 0; I <= NSEG; I++)
                                    {
                                        VREP[I] = REP[I] * IPPS[IN];
                                    }

                                    // AJUSTEMENT DU MILIEU DES MARCHES
                                    R = 1;
                                    LAMBDA = 0.5f;
                                    DELTA = 0.05f;
                                    SGN0 = 0;
                                    RSAVE = R;

                                    // ITERATIONS SUR AFFECTATIONS
                                    //45               
                                    while (LAMBDA > 0.05 && LAMBDA < 0.95) //GO TO 75
                                    {
                                        GT[N1] = 0f;
                                        R = RSAVE;
                                        X = 1E-6;
                                        RP = R - 1;

                                        // AFFECTATION DU SEGMENT ZERO
                                        bool bug = false;
                                        do
                                        {
                                            RP += 1;
                                            H = RANK[RP];

                                            if (RP > NICEL)
                                            {
                                                sbSortie.AppendLine($"Bug dans CALDISTR {IP},{IU},{IQ},{N1}");
                                                bug = true;
                                                break;
                                            }
                                            X = X + POIDS[H];
                                        }
                                        while (X < VREP[0]);

                                        if (!bug)
                                        {

                                            if (VREP[0] > 1E-6 && (RP > R) && (X > (VREP[0] + LAMBDA * POIDS[H])))
                                            {
                                                X -= POIDS[H];
                                                RP--;
                                            }

                                            PR = 0;
                                            PT1[N1] = PTNSEG[N1] = RP + 1;

                                            // AFFECTATION DES PROBAS
                                            for (; ; )
                                            {
                                                while ((R <= RP) && (R <= NBIND))
                                                {
                                                    H = RANK[R];
                                                    PROBAS[NOP, IU, IQ, H] = PR;
                                                    if (PR > 0)
                                                    {
                                                        NG = PR * POIDS[H];
                                                        GT[N1] += NG;
                                                    }

                                                    R++;
                                                }

                                                if (R > NICEL) break;
                                                // RECHERCHE PROCHAIN SAUT

                                                RP = R;
                                                NG = POIDS[RANK[R]];
                                                HCR = (int)Math.Truncate(SCORE[RANK[R]]);  // TODO Real => INT*4 ?
                                                                                           //60
                                                double HCRf =SCORE[RANK[R]];
                                                do
                                                {
                                                    H = RANK[RP + 1];
                                                    //if (SCORE[H] != HCR) break;
                                                    if (SCORE[H] != HCRf) break;

                                                    RP = RP + 1;
                                                    NG = NG + POIDS[H];
                                                }
                                                while (RP < NICEL);

                                                Y = X + NG * LAMBDA;
                                                X = X + NG;

                                                while (VREP[PR] < Y && PR < NSEG) PR++;

                                                if (PR > 0 && (Math.Abs(VREP[PR - 1] - Y)) < (Math.Abs(VREP[PR] - Y))) PR--;
                                                if (PR < NSEG) PTNSEG[N1] = RP + 1;
                                            }

                                            GT[N1] /= 1d*(NSEG * IPPS[IN]);
                                            double Z = (GRPN - GT[N1]) * NSEG * IPPS[IN];
                                            float K = PTNSEG[N1] - PT1[N1];

                                            // TEST SI GRP COHERENT
                                            if (Math.Abs(Z) >= (K / 4d + 1d) && (DELTA >= 1E-3))
                                            {
                                                SGN = Z < 0 ? -1d : 1d;
                                                if (SGN == -SGN0) DELTA *= 0.3d;
                                                LAMBDA += SGN * DELTA;
                                                SGN0 = SGN;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            break;  // GOTO 75
                                        }
                                    } // while (LAMBDA <= 0.05 || LAMBDA >= 0.95)  ==> GO TO 45

                                    // 75
                                    GRP = 0f;
                                    NPERR = 0;
                                    for (int I = 1; I <= NICEL; I++)
                                    {
                                        H = RANK[I];
                                        if (PROBAS[NOP, IU, IQ, H] != 255)
    //                                    if (PROBAS[NOP, IU, IQ, H] >= 0)
                                        {
                                            GRP += POIDS[H] * PROBAS[NOP, IU, IQ, H];
                                        }
                                        else
                                        {
                                            NPERR++;
                                            PROBAS[NOP, IU, IQ, H] = 0;
                                        }
                                    }

                                    GRP = GRP / Convert.ToSingle(IPPS[IN]) / 100f;
                                    sbSortie.AppendLine(IQ.ToString() + $" OBJECTIF GRP={GRPN}   GRP TROUVE ={GRP}");

                                    //FIN DE TRAITEMENT CELLULE
                                }
                            } // for (int N1=1; N1 <=16; N1++) 

                        } // if (REGRS[IU, IP, IQ, 1] == 5)

                        if (IP != 999)
                        {
                            for (int IG = 1; IG <= NBIND; IG++)
                                writeBinary.Write(PROBAS[NOP, IU, IQ, IG]);
                        }

                    } // FIN DE BOUCLE 1/4h

                } // FIN DE BOUCLE UNIVERS

            } // FIN DE BOUCLE STATION

            if (File.Exists(pathSortie)) File.Delete(pathSortie);
            File.AppendAllText(pathSortie, sbSortie.ToString());
            
            writeBinary.Close();
            writeStream.Close();

            return PROBAS;
        }


        private BSupport transp08(int NIND, int NBSTA, int NBSTAIDF, int[] ISTA, int[,] POIDSEGS, List<int> FILT, List<int> POIDS, byte[,,,] KHI2,
            string pathSortie, string pathYearNat, string pathYearIdf, string pathYearSup)
        {
            System.GC.Collect();
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // TRANSFORMATION DES PROBABILITES INDIVIDUELLES
            //   DE 100e EN 1000e ET DE 1 / 4h EN 1 / 2h
            int[][,,] KECR = new int[NBSTA+1][,,]; // , 3 + 1, 48 + 1, NIND + 1];
            for (int s = 1; s <= NBSTA; s++) KECR[s] = new int[3 + 1, 48 + 1, NIND + 1];
            //int[,,,] KECR = new int[NBSTA + 1, 3// + 1, 48 + 1, NIND + 1];
            int[][,,] KECRIDF = new int[NBSTAIDF + 1][,,];
            for (int s = 1; s <= NBSTAIDF; s++) KECRIDF[s] = new int[3 + 1, 48 + 1, NIND + 1];
            //int[,,,] KECRIDF = new int[NBSTAIDF + 1, 3 + 1, 48 + 1, NIND + 1];
            int[] RIEN = new int[NIND + 1];
            BSupport BSUP = new BSupport();

            //      INITIALISATIONS

            if (File.Exists(pathYearNat)) File.Delete(pathYearNat);
            if (File.Exists(pathYearIdf)) File.Delete(pathYearIdf);
            if (File.Exists(pathYearSup)) File.Delete(pathYearSup);

            BinaryWriter bwNat = new BinaryWriter(File.Create(pathYearNat));
            BinaryWriter bwIdf = new BinaryWriter(File.Create(pathYearIdf));
            BinaryWriter bwSup = new BinaryWriter(File.Create(pathYearSup));

            // COEFF.ET DEPARTEMENT
            for (int IND = 0; IND < NIND; IND++)
            {
                POIDS[IND] *= 10;
                RIEN[IND] = 0;
            }
            for (int IND =0; IND < NIND; IND++) bwNat.Write(Convert.ToInt16(POIDS[IND]));
            for (int IND = 0; IND < NIND; IND++) bwNat.Write(Convert.ToInt16(RIEN[IND]));

            // BOUCLE STATIONS
            int IDF = 0;
            for (int IS = 1; IS <= NBSTA; IS++)
            {
                int IFO = ISTA[IS];
                if (IFO == 1)
                {
                    IDF++;
                }

                Console.WriteLine($"Traitement station {IS}");

                // BOUCLE UNIVERS

                for (int IU = 1; IU <= 3; IU++)
                {
                    // BOUCLE 1 / 4h - PASSAGE A LA 1 / 2h
                    for (int IQ = 1; IQ <= 96; IQ += 2)
                    {
                        int IH = IQ + 20;
                        if (IH > 96) IH = IH - 96;
                        int IQ4 = (IQ + 1) / 2; 
                        // BOUCLE INDIVIDUS
                        for (int II = 1; II <= NIND; II++)
                        {
                            KECR[IS][IU, IQ4, II] = KHI2[IS, IU, IH, II] + KHI2[IS, IU, IH + 1, II];      //  [STATIONS, LV/Sa/Di, QH, INDIVS]

                            KECR[IS][IU, IQ4, II] *= 5;

                            if (KECR[IS][IU, IQ4, II] < 0) Console.WriteLine($"{IS}, {IU}, {IQ4}");

                            bwNat.Write(Convert.ToInt16(KECR[IS][IU, IQ4, II]));
                        }

                        // ECRITURE FICHIER IDF
                        if (IFO == 1)
                        {
                            for (int IN = 1; IN <= NIND; IN++)
                            {
                                if (FILT[IN-1] == 0)
                                    KECRIDF[IDF][IU, IQ4, IN] = 0;
                                else
                                    KECRIDF[IDF][IU, IQ4, IN] = KECR[IS][IU, IQ4, IN];

                                bwIdf.Write(Convert.ToInt16(KECRIDF[IDF][IU, IQ4, IN]));
                            }
                        }
                    }
                }
            }

            BSUP.POIDS = POIDS;
            BSUP.RIEN = RIEN;
            BSUP.KECR = KECR;
            BSUP.KECRIDF = KECRIDF;

            for (int IND = 0; IND < NIND; IND++) bwSup.Write(Convert.ToInt16(POIDS[IND]));
            for (int IND = 0; IND < NIND; IND++) bwSup.Write(Convert.ToInt16(RIEN[IND]));

            for (int IS = 1; IS <= NBSTA; IS++)
            {
                // BOUCLE UNIVERS
                for (int IU = 1; IU <= 3; IU++)
                {
                    for (int IQ = 1; IQ <= 48; IQ ++)
                    {
                        for (int II = 1; II <= NIND; II++)
                        {
                            bwSup.Write(Convert.ToInt16(KECR[IS][IU, IQ, II]));
                        }
                    }
                }
            }
            for (int IS = 1; IS <= NBSTAIDF; IS++)
            {
                // BOUCLE UNIVERS IDF
                for (int IU = 1; IU <= 3; IU++)
                {
                    for (int IQ = 1; IQ <= 48; IQ ++)
                    {
                        for (int II = 1; II <= NIND; II++)
                        {
                            bwSup.Write(Convert.ToInt16(KECRIDF[IS][IU, IQ, II]));
                        }
                    }
                }
            }

            bwNat.Close();
            bwIdf.Close();
            bwSup.Close();

            return BSUP;
        }

        private int[,] crecib08(int NIND, List<Fushab09Indiv> fushab09Indivs, int COL_AGE, int COL_MENA, string pathPan20Cib)
        {
            // PANEL RADIO 08 MEDIAMETRIE
            // CREATION DE VECTEURS DE CIBLES DE CONTROLE ASYMPTOTES
            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)

            //C Attention AVANT(#SIGN_LINE_LEN_BEFORE_HAB#) et le buffer de lecture pour se caler au début des données de HAB
            //C Attention APRES(#SIGN_LINE_LEN_AFTER_HAB#) et le buffer de lecture pour se caler à la fin de la ligne des données

            int[,] KECR = new int[3 + 1, NIND + 1];

            // INITIALISATIONS
            int IG = 0;
            int CPT2 = 0;
            int CPT3 = 0;

            foreach (Fushab09Indiv fushab09Indiv in fushab09Indivs)
            {
                IG = IG + 1;
                int IND = IG;
                // Ensemble(13 ans et +)
                KECR[1, IND] = 1;
                //C Ménagères moins de 50 ans
                KECR[2, IND] = 0;
                //C 13 - 29 ans
                KECR[3, IND] = 0;

                // Construction de la cible des Ménagères moins de 50 ans
                int AGE = 10 * (fushab09Indiv.AVANT[COL_AGE] - 48) + (fushab09Indiv.AVANT[COL_AGE + 1] - 48);
                if (((fushab09Indiv.AVANT[COL_MENA] - 48) == 1) && (AGE <= 7))
                {
                    KECR[2, IND] = 1;
                    CPT2 = CPT2 + 1;
                }

                // Construction de la cible des 13 - 29 ans
                if (AGE <= 3)
                {
                    KECR[3, IND] = 1;
                    CPT3 = CPT3 + 1;
                }
            }

            Console.WriteLine(IG.ToString());

            if (File.Exists(pathPan20Cib)) File.Delete(pathPan20Cib);
            BinaryWriter bwNat = new BinaryWriter(File.Create(pathPan20Cib));
            for (int I = 1; I <= 3; I++)
            {
                for (int J = 1; J <= NIND; J++)
                {
                    bwNat.Write(Convert.ToInt16(KECR[I, J]));
                }
            }
            bwNat.Close();

            Console.WriteLine($"CPT2 = {CPT2}");
            Console.WriteLine($"CPT3 = {CPT3}");

            return KECR;
        }


        private void penetr(int NBIND, int NBSTA, VsorPoid[][] JN, List<int> POIDS, string pathPenetr, int population, List<string> strStations)
        {
            // PANEL RADIO 08 MEDIAMETRIE
            // ACCUMULATION D'AUDIENCE 3 SEMAINES (0-24h)

            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) (avec Total Radio et Total TV)
            int NBJOUR = 23;
            ushort[,,] VSOR2 = new ushort[NBJOUR + 1, 6+1, NBSTA + 1];
            int[] ITAP = new int[NBSTA + 1];
            int[,] FLAG = new int[NBSTA + 1, NBIND + 1];
            
            // INITIALISATIONS

            for (int I = 1; I <= NBJOUR; I++)
            {
                // BOUCLE INDIVIDUS
                for (int IG = 1; IG <= NBIND; IG++)
                {
                    for (int J = 1; J <= 6; J++)
                    {
                        for (int K = 1; K <= NBSTA; K++)
                        {
                            VSOR2[I, J, K] = JN[I][IG].VSor[J, K];  // [Jour 1..23][Individus 1..N] . VSor[1..6, STATIONS]
                        }
                    }

                    for (int K = 1; K <= NBSTA; K++)
                    {
                        for (int M = 1; M <= 6; M++)
                        {
                            for (int L = 1; L <= 16; L++)
                            {
                                ushort[] bits = new ushort[7];
                                for (int b = 1; b <= 6; b++)
                                    bits[b] = VSOR2[I,M,K]; //  JN[I][IG].VSor[b, K];

                                if (L1BITFCT(bits, L))
                                {
                                    FLAG[K, IG] = 1;
                                }
                            }
                        }
                    }

                }
            }

            if (File.Exists(pathPenetr)) File.Delete(pathPenetr);
            StreamWriter swPenetr = new StreamWriter(File.Create(pathPenetr));
            for (int K = 1; K <= NBSTA; K++)
            {
                for (int IG = 1; IG <= NBIND; IG++)
                {
                    ITAP[K] += FLAG[K, IG] * POIDS[IG-1];
                }
                swPenetr.WriteLine(strStations[K-1].PadLeft(49,' ') + "," + ((100d * ITAP[K]) / population).ToString("0.00"));
            }
            swPenetr.Close();
        }


        private void asympt(int NIND, int NBSTA, int NBSTATOTAL, BSupport BSUP, int[,] PANCIB, string pathAS5H5H, string headerAS5H5H, List<string> stations)
        {
            // PANEL RADIO 08 MEDIAMETRIE
            // PROGRAMME ASYMPT
            // PENETRATIONS CUMULEES MAXI 5H - 29H LUNDI-DIMANCHE

            int N = 3;

            // Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)
            // Il ne nous reste plus que #NB_STA_HAB_NOTO,0# stations puisqu'on a supprimé SUD RADIO
            int[] IPOI = new int[NIND + 1];
            int[] IPOPS = new int[N + 1];
            float[] COUV = new float[N + 1];
            int[,,] TABRES = new int[N + 1, NBSTATOTAL  + 1, NIND + 1];
            int[] IG = new int[N + 1];

            // OUVERTURE DES FICHIERS

            //      OPEN(7, FILE = '#OUTPUT#PANRA1#YEAR#.SUP',
            //     -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED', RECL = 2 * NIND)
            //      READ(7) IPO2
            //      READ(7) VECT

            for (int IND = 1; IND <= NIND; IND++)
            {
                IPOI[IND] = BSUP.POIDS[IND-1];
                //if (IPOI[IND] < 0) IPOI[IND] += 65536;  /// ???
            }

            // FICHIER DES SEGMENTS
            //      OPEN(8, FILE = '#OUTPUT#PAN#YEAR#CIB',
            //     -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED', RECL = 2 * NIND)
            for (int I = 1; I <= N; I++)
            {
                //      READ(8)(TABCI(I, IND), IND = 1, NIND)
                IPOPS[I] = 0;
                IG[I] = 0;
                for (int IND = 1; IND <= NIND; IND++)
                {
                    if (PANCIB[I, IND] == 1)
                    {
                        IPOPS[I] += IPOI[IND];
                        IG[I] = IG[I] + 1;
                    }
                }
            }
            //      CLOSE(8)


            // BOUCLE STATIONS
            for (int IS = 1; IS <= NBSTA; IS++)
            { 
                // BOUCLE SEMAINE SAMEDI DIMANCHE
                for (int IU = 1; IU <= 3; IU++)
                { 
                    // BOUCLE TRANCHES HORAIRES(EN 1 / 2 HEURE)
                    for (int ITR = 1; ITR <= 48; ITR++)
                    { 
                        //  TRANCHE HORAIRE 5h 29h
                        // BOUCLE INDIVIDUS
                        for (int IND = 1; IND <= NIND; IND++)
                        {
                            if (BSUP.KECR[IS][IU, ITR, IND] > 0)  // [NBSTA + 1, 3 + 1, 48 + 1, NIND + 1]
                            {
                                for (int IN = 1; IN <= N; IN++)
                                {
                                    if (PANCIB[IN, IND] == 1) TABRES[IN, IS, IND] = 1;
                                    if (PANCIB[IN, IND] == 1) TABRES[IN, NBSTATOTAL, IND] = 1;
                                }
                            }
                        }
                    }
                }
            }

            Console.WriteLine("IPOPS");
            for (int I = 1; I <= N; I++)
                Console.WriteLine(IPOPS[I].ToString());

            if (File.Exists(pathAS5H5H)) File.Delete(pathAS5H5H);
            StreamWriter swPenetr = new StreamWriter(File.Create(pathAS5H5H));
            // Header
            swPenetr.WriteLine(headerAS5H5H);

            for (int STA = 1; STA <= NBSTA; STA++)
            {
                for (int I = 1; I <= N; I++)
                {
                    for (int IND = 1; IND <= NIND; IND++)
                    {
                        if (TABRES[I, STA, IND] == 1) COUV[I] += IPOI[IND];
                    }
                    COUV[I] *= 100f / Convert.ToSingle(IPOPS[I]);
                }
                swPenetr.WriteLine(stations[STA-1].PadLeft(49) + COUV[1].ToString("0.00").PadLeft(8) + " " + COUV[2].ToString("0.00").PadLeft(8) + " " + COUV[3].ToString("0.00").PadLeft(8));
            }


            for (int I = 1; I <= N; I++)
            {
                for (int IND = 1; IND <= NIND; IND++)
                {
                    if (TABRES[I, NBSTATOTAL, IND] == 1) COUV[I] += IPOI[IND];
                }
                COUV[I] *= 100f / IPOPS[I];
            }
            swPenetr.WriteLine(" Total".PadRight(49) + COUV[1].ToString("0.00").PadLeft(8) + " " + COUV[2].ToString("0.00").PadLeft(8) + " " + COUV[3].ToString("0.00").PadLeft(8));
            swPenetr.Close();
        }

        public void SORTF(int I1, int IFIN, int[] RANG, double[] SCORE)
        {
            double X;
            int I = I1;
            int J = IFIN;

            X = SCORE[RANG[(I1 + IFIN) / 2]];
            do
            {
                while (SCORE[RANG[I]] < X)
                    I = I + 1;

                while (SCORE[RANG[J]] > X)
                    J = J - 1;

                if (I <= J)
                {
                    int K = RANG[I];
                    RANG[I] = RANG[J];
                    RANG[J] = K;
                    I = I + 1;
                    J = J - 1;
                }
            }
            while (I <= J);

            if (I1 < J) SORTF(I1, J, RANG, SCORE);
            if (I < IFIN) SORTF(I, IFIN, RANG, SCORE);
        }

        // CALCUL DE LA REPARTITION DES PROBAS
        public void CALDISTR(float ZR, float UR, float PO, float TAUX, double[] LOGS, out double[] DISTR, out double[] REP)
        {
            int NSEG = 100;
            int JMAX = NSEG * 2;
            int JSUP = JMAX + 2;
            double ALPHA, BETA, X, V, Q, Y, BAB, LEBND, Z, U, P, TAU;

            double[] BUFS = new double[JMAX + 1];
            DISTR = new double[NSEG + 1];
            REP = new double[NSEG + 1];

            Z = ZR;
            U = UR;
            P = PO;
            TAU = TAUX;

            // ECRETAGES
            if (P < 1E-4) TAU = 0;
            if (P < 1E-4) P = 0;
            if (P > (1d - (1E-4))) TAU = 0;
            if (P > (1d - (1E-4))) P = 1;

            // CALCUL DES PARAMETRES DU MODELE
            Q = 1f - P;
            if (TAU <= 1E-4)
            {
                TAU = 0;
                ALPHA = (double)(P * 1E4);
                BETA = (double)(Q * 1E4);
            }
            else
            {
                ALPHA = P / TAU;
                BETA = Q / TAU;
            }

            if (TAU < 0.003)
            {
                for (int I = 0; I <= NSEG; I++)
                    DISTR[I] = 0;

                if (P > 0.998)
                    DISTR[NSEG] = 1;
                else if (P < 0.002)
                    DISTR[0] = 1;
                else
                    DISTR[(int)Math.Truncate(P * (NSEG + 1))] = 1;
            }
            // TAU > ou = 0.003
            else
            {
                for (int J = 2; J <= JMAX; J++)
                    BUFS[J] = (double) Math.Exp((ALPHA - 1) * LOGS[J] + (BETA - 1) * LOGS[JSUP - J]);
                // ASTUCE POUR LES EXTREMITES
                LEBND = LOGS[2];
                X = 1d / (6d * (NSEG + 1d));
                Y = 1d - (BETA - 1d) / JSUP;
                if (Y < 0.8d) Y = 0.8d;
                if (Y > 1.2d) Y = 1.2d;
                DISTR[0] = Y * (double)Math.Exp(ALPHA * LEBND) / ALPHA;
                Y = 1d - (ALPHA - 1d) / Convert.ToDouble(JSUP);

                if (Y < 0.8d) Y = 0.8d;
                if (Y > 1.2d) Y = 1.2d;
                DISTR[NSEG] = Y * (double)Math.Exp(BETA * LEBND) / BETA;
                // SEGMENTS INTERMEDIAIRES
                for (int I = 1; I <= NSEG - 1; I++)
                    DISTR[I] = (BUFS[2 * I] + BUFS[2 + 2 * I] + 4 * BUFS[1 + 2 * I]) * X;
                // NORMALISATION DE DISTR
                BAB = 0d;
                for (int I = 0; I <= NSEG; I++)
                    BAB += DISTR[I];
                for (int I = 0; I <= NSEG; I++)
                    DISTR[I] /= BAB;
            }

            // CORRECTION DES Z ET U
            V = 1f - Z - U;
            for (int I = 0; I <= NSEG; I++)
                DISTR[I] *= V;
            DISTR[0] += Z;
            DISTR[NSEG] += U;
            REP[0] = DISTR[0];
            for (int I = 1; I <= NSEG; I++)
                REP[I] = REP[I - 1] + DISTR[I];
        }

        public int[] GET_STA_IDF_LIST_NO_SUDRAD_MASK(ARProba arProba)
        {
            int[] sta = new int[arProba.HabAndNotoStationList.Count() + 1];
            int i = 1;
            foreach (var station in arProba.HabAndNotoStationList)
            {
                if (!station.Name.ToUpper().Equals("SUD RADIO"))
                {
                    sta[i++] = station.IsIdf?1:0;
                }
            }
            return sta;
        }

        public int[] HAB_STA_LIST_ID_NOTO_SET_TO_TOTAL_RADIO(ARProba arProba)
        {
            int[] sta = new int[arProba.HabAndNotoStationList.Count() + 1];
            int totalidx = GetTotalRadioIndex(arProba);
            int i = 1;
            foreach (var station in arProba.HabAndNotoStationList)
            {
                if (station.Mode == ARProba.Station.eSignVariable.Habit)
                    sta[i] = station.Index;
                else
                    sta[i] = totalidx;
                i++;
            }
            return sta;

        }
        private int GetTotalRadioIndex(ARProba arProba)
        {
            var req = from sta in arProba.HabAndNotoTotalStationList
                      where sta.Name.Equals("total radio", StringComparison.CurrentCultureIgnoreCase)
                      select sta;

            return req.First().Index;
        }

        private int SIGN_LINE_LEN_BEFORE_HAB_FCT(ARProba arProba)
        {
            int len = arProba.SignVars["LV01"].Position - 1;
            return len;
        }

        private int SIGN_LINE_LEN_AFTER_HAB_FCT(ARProba arProba)
        {
            int len = arProba.SigLineLen;

            var var = arProba.SignVars[string.Format("DI{0:00}", arProba.AllHabStationCount)];
            len = len - (var.Position + var.Repetition) + 1;

            return len;
        }

        public int[] HAB_0_NOTO_1_STA_LIST_MASK(ARProba arProba)
        {
            int[] res = new int[arProba.HabAndNotoStationCount + 1];
            int count = 1;
            foreach (var station in arProba.HabAndNotoStationList)
            {
                if (station.Mode == ARProba.Station.eSignVariable.Habit)
                    res[count] = 0;
                else
                    res[count] = 1;
                count++;
            }
            return res;
        }

        public List<Tuple<int,int>> CRENONIN_NOTO_HANDLING(ARProba arProba)
        {
            List<Tuple<int, int>> stationApres = new List<Tuple<int, int>>();

            var varLastDI = arProba.SignVars[string.Format("DI{0:00}", arProba.AllHabStationCount)];
            int offset = (varLastDI.Position + varLastDI.Repetition);

            int count = 0;
            foreach (var station in arProba.HabAndNotoStationList)
            {
                count++;
                if (station.Mode == ARProba.Station.eSignVariable.Notoriety)
                {
                    int notoNum = station.Index / 100;
                    int notoMod = station.Index % 100;

                    var ques = arProba.SignVars[string.Format("NOTO{0}", notoNum)];

                    int pos = ques.Position + notoMod - 1;

                    stationApres.Add(new Tuple<int, int>(count, pos - offset + 1));
                }
            }
            return stationApres;
        }

        //public void COLAPRES(StringBuilder line, string colName, string idx)
        //{
        //    int pos = aRProba.SignVars[colName].Position;
        //    int index = int.Parse(idx);

        //    var varLastDI = aRProba.SignVars[string.Format("DI{0:00}", aRProba.AllHabStationCount)];
        //    int offset = (varLastDI.Position + varLastDI.Repetition);

        //    line.Append(pos + index - offset + 1);
        //}


        private int GetCOLAPRES(ARProba arProba, string colName)
        {
            int pos = arProba.SignVars[colName].Position;

            var varLastDI = arProba.SignVars[string.Format("DI{0:00}", arProba.AllHabStationCount)];
            int offset = (varLastDI.Position + varLastDI.Repetition);

            return pos - offset + 1;
        }

        public int[] HAB_NOTO_STA_LIST_U1XX_INDEXES(ARProba arProba)
        {
            int[] res = new int[arProba.HabAndNotoStationList.Count() + 1];
            int i = 1;
            foreach (var station in arProba.HabAndNotoStationList)
            {
                res[i++] = station.U1xxIdx;
            }
            return res;
        }

        private void GetIndiceUXXX(string pathUxxxDesc, Enquete enquete, out int IND_CSP, out int IND_AGE, out int IND_SEX, out int IND_REG)
        {
            // Pour pannat 2020
            IND_CSP = -1;
            IND_AGE = -1;
            IND_SEX = -1;
            IND_REG = -1;
            string[] lines = File.ReadAllLines(pathUxxxDesc, Encoding.GetEncoding("iso-8859-1"));
            foreach (string s in lines)
            {
                if (s.Contains(": Sexe :"))
                {
                    IND_SEX = int.Parse(s.Substring(0, s.IndexOf(':')))+2;
                }
                else if ((enquete == Enquete.PanelNational && s.Contains("CSP Individu") && s.Contains("7 cl.")) ||
                    (enquete == Enquete.PanelCadre && s.Contains("CSP Individu")))
                {
                    IND_CSP = int.Parse(s.Substring(0, s.IndexOf(':'))) + 2;
                }
                else if ((enquete == Enquete.PanelNational && s.Contains("Age") && s.Contains("12 cl.")) ||
                         (enquete == Enquete.PanelCadre && s.Contains("Age sur 7 Tranches")))
                {
                    IND_AGE = int.Parse(s.Substring(0, s.IndexOf(':'))) + 2;
                }
                else if (s.Contains(": Régions UDA :"))
                {
                    IND_REG = int.Parse(s.Substring(0, s.IndexOf(':'))) + 2;
                }
                if (IND_SEX != -1 && IND_CSP != -1 && IND_AGE != -1 && IND_REG != -1) break;
            }

            if (IND_SEX == -1)
                throw new Exception("GetIndiceUXXX : indice 'Sexe' pas trouve dans " + pathUxxxDesc);
            if (IND_CSP == -1)
                throw new Exception("GetIndiceUXXX : indice 'CSP Individu' pas trouve dans " + pathUxxxDesc);
            if (IND_AGE == -1)
                throw new Exception("GetIndiceUXXX : indice 'Age (12 cl.)' pas trouve dans " + pathUxxxDesc);
            if (IND_REG == -1)
                throw new Exception("GetIndiceUXXX : indice 'Régions UDA' pas trouve dans " + pathUxxxDesc);
        }
    }

    public struct BSupport
    {
        public List<int> POIDS;
        public int[] RIEN;
        public int[][,,] KECR;
        public int[][,,] KECRIDF;
    }
}

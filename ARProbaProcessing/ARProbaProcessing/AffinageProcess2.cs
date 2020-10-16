using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ARProbaProcessing
{
    public partial class AffinageProcess
    {
        private int[,,,] attribp2(int NBIND, int NBSTA, int[,,,] REGRS, int[,] KHI2, int[,] NINI, double[] NOTES, int[,,,] AUDI, int[,,] HABI,
            double[,,,] ZUPTAUSECOR, double[,,,] ZUPTAUSACOR, double[,,,] ZUPTAUDICOR, string pathSortie)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // ATTRIBUTION DES PROBAS 1/4h PAR 1/4h,L-V,SAM,DIM
            //        STATION PAR STATION
            // Le nombre de station correspond au nombre de stations (#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)
            int[,,,] PROBAS = new int[NBSTA + 1, 3 + 1, 96 + 1, NBIND + 1];
            int NSEG = 100;
            int JMAX = NSEG * 2;
            int JSUP = JMAX + 2;

            double[] LOGS = new double[JMAX + 1];
            double[] DISTR = new double[NSEG + 1];
            double[] REP = new double[NSEG + 1];
            double[] VREP = new double[NSEG + 1];
            double[] GT = new double[16 + 1];

            double XJ, LAMBDA, DELTA, PCUM, NG, GRPN, X, Y;
            double[] SCORE = new double[NBIND + 1];
            int[] TAB = new int[NBIND + 1];
            double NOTI, SGN, SGN0;
            int[] POIDS = new int[NBIND + 1];
            int[] IPPS = new int[5 + 1];
            int R, RSAVE, RP, H, HCR, PR;
            int[] NB = new int[5 + 1];
            int[] IM = new int[5 + 1];
            int[] RANK = new int[NBIND + 1 + 1];
            int[] RANG = new int[NBIND + 1];
            int[] CELL = new int[NBIND + 1];
            int[] PT1 = new int[16 + 1];
            int[] PTNSEG = new int[16 + 1];
            int[] TREG = new int[16 + 1];

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
                KHI2[IG, 6] = 1;
                IPERS = IPERS * 10;
                POIDS[IG] = IPERS;
            }
            // BOUCLE STATIONS
            for (int NOP = 1; NOP <= NBSTA; NOP++)
            {
                Console.WriteLine($"Traitement station {NOP}");

                // BOUCLE UNIVERS
                for (int IU = 1; IU <= 3; IU++)
                {
                    double[,,,] RESUL = null;
                    // BOUCLE 1/4h
                    for (int IQ = 1; IQ <= 96; IQ++)
                    {

                        if (IU == 1) RESUL = ZUPTAUSECOR;
                        if (IU == 2) RESUL = ZUPTAUSACOR;
                        if (IU == 3) RESUL = ZUPTAUDICOR;

                        // TRI DES SCORES INDIVIDUELS
                        for (int I = 1; I <= NBIND; I++)
                        {
                            SCORE[I] = AUDI[NOP, I, IQ, IU];   // [STATIONS, INdiv, QH, 1..3]

                            if (SCORE[I] > 0) SCORE[I] = SCORE[I] + 256;
                            SCORE[I] = 10 * SCORE[I] + HABI[I, IQ, IU];

                            NOTI = NOTES[I];
                            if (NINI[I, NOP] == 1) NOTI = 0d;
                            SCORE[I] = 1000 * SCORE[I] + NOTI;
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
                        if (REGRS[IU, IP, IQ, 1] == 5)
                        {
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
                                        if (KHI2[IG, NIV + 1] == NB[NIV])
                                        {
                                            IM[NIV]++;
                                            IPPS[NIV] = IPPS[NIV] + POIDS[IG];
                                            PROBAS[NOP, IU, IQ, IG] = -1;
                                            II = II + 1;
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
                                    double ZR = RESUL[NOP, IQ, 1, N1];
                                    double PGR = 0d;
                                    double GRP = 0d;
                                    int NPERR = 0;

                                    if (ZR >= 1d)
                                    {
                                        for (int I = 1; I <= NICEL; I++)
                                        {
                                            H = RANK[I];
                                        }
                                        PGR = 0d;
                                        GRP = 0;
                                        NPERR = 0;
                                        break; // Fin parcours Cellules
                                    }

                                    double UR = RESUL[NOP, IQ, 2, N1];
                                    double P = RESUL[NOP, IQ, 3, N1];
                                    double TAU = RESUL[NOP, IQ, 4, N1];
                                    GRPN = UR + P * (1d - ZR - UR);

                                    CALDISTR(ZR, UR, P, TAU, out DISTR, out REP);

                                    // IMPRESSION DES RESULTATS
                                    int IN = TREG[N1] + 1;

                                    // REPARTITION DANS LA CELLULE
                                    for (int I = 0; I <= NSEG; I++)
                                    {
                                        VREP[I] = REP[I] * IPPS[IN];
                                    }

                                    // AJUSTEMENT DU MILIEU DES MARCHES
                                    R = 1;
                                    LAMBDA = 0.5;
                                    DELTA = 0.05;
                                    SGN0 = 0;
                                    RSAVE = R;

                                    // ITERATIONS SUR AFFECTATIONS
                                    //45               
                                    while (LAMBDA <= 0.05 || LAMBDA >= 0.95) //GO TO 75
                                    {
                                        GT[N1] = 0d;
                                        R = RSAVE;
                                        X = 1E-6;
                                        RP = R - 1;
                                        PCUM = 0d;

                                        // AFFECTATION DU SEGMENT ZERO
                                        bool bug = false;
                                        do
                                        {
                                            RP = RP + 1;
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

                                            if (VREP[0] > 1E-6 && RP > R && X > (VREP[0] + LAMBDA * POIDS[H]))
                                            {
                                                X = X - POIDS[H];
                                                RP = RP - 1;
                                            }

                                            PR = 0;
                                            PT1[N1] = RP + 1;
                                            PTNSEG[N1] = RP + 1;

                                            // AFFECTATION DES PROBAS
                                            for (; ; )
                                            {
                                                while (R <= RP && (R <= NBIND))
                                                {
                                                    H = RANK[R];
                                                    PROBAS[NOP, IU, IQ, H] = PR;
                                                    if (PR > 0)
                                                    {
                                                        NG = PR * POIDS[H];
                                                        PCUM = PCUM + POIDS[H];
                                                        GT[N1] = GT[N1] + NG;
                                                    }

                                                    R = R + 1;
                                                }

                                                if (R > NICEL) break;
                                                // RECHERCHE PROCHAIN SAUT

                                                RP = R;
                                                NG = POIDS[RANK[R]];
                                                HCR = (int)Math.Truncate(SCORE[RANK[R]]);  // TODO Real => INT*4 ?
                                                                                           //60
                                                do
                                                {
                                                    H = RANK[RP + 1];
                                                    if (SCORE[H] != HCR) break;

                                                    RP = RP + 1;
                                                    NG = NG + POIDS[H];
                                                }
                                                while (RP < NICEL);

                                                Y = X + NG * LAMBDA;
                                                X = X + NG;

                                                while (VREP[PR] < Y && PR < NSEG)
                                                {
                                                    PR = PR + 1;
                                                }

                                                if (PR > 0 && (Math.Abs(VREP[PR - 1] - Y)) < (Math.Abs(VREP[PR] - Y))) PR = PR - 1;
                                                if (PR < NSEG) PTNSEG[N1] = RP + 1;
                                            }

                                            GT[N1] /= (NSEG * IPPS[IN]);
                                            double Z = (GRPN - GT[N1]) * NSEG * IPPS[IN];
                                            int K = PTNSEG[N1] - PT1[N1];

                                            // TEST SI GRP COHERENT
                                            if (Math.Abs(Z) >= (K / 4 + 1) && (DELTA >= 1E-3))
                                            {
                                                SGN = Z < 0 ? -1d : 1d;

                                                if (SGN == -SGN0) DELTA *= 0.3;
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
                                    PGR = 0d;
                                    GRP = 0d;
                                    NPERR = 0;
                                    for (int I = 1; I <= NICEL; I++)
                                    {
                                        H = RANK[I];
                                        if (PROBAS[NOP, IU, IQ, H] >= 0)
                                        {
                                            PGR = PGR + POIDS[H];
                                            GRP = GRP + POIDS[H] * PROBAS[NOP, IU, IQ, H];
                                        }
                                        else
                                        {
                                            NPERR = NPERR + 1;
                                            PROBAS[NOP, IU, IQ, H] = 0;
                                        }
                                    }

                                    GRP = GRP / IPPS[IN] / 100;
                                    sbSortie.AppendLine(IQ.ToString() + $" OBJECTIF GRP={GRPN}   GRP TROUVE ={GRP}");

                                    //FIN DE TRAITEMENT CELLULE
                                }
                            } // for (int N1=1; N1 <=16; N1++) 

                        } // if (REGRS[IU, IP, IQ, 1] == 5)
                          // TODO if (IP != IDXSUDRAD) WRITE(30) PROBAS

                    } // FIN DE BOUCLE 1/4h

                } // FIN DE BOUCLE UNIVERS

            } // FIN DE BOUCLE STATION

            if (File.Exists(pathSortie)) File.Delete(pathSortie);
            File.AppendAllText(pathSortie, sbSortie.ToString());

            return PROBAS;
        }


        private BSupport transp08(int NIND, int NBSTA, int NBSTAIDF, int[] ISTA, int[,] POIDSEGS, List<int> FILT, List<int> POIDS, int[,,,] KHI2,
            string pathSortie, string pathYearNat, string pathYearIdf, string pathYearSup)
        {
            // PANEL RADIO 08 MEDIAMETRIE(nouveau format)
            // TRANSFORMATION DES PROBABILITES INDIVIDUELLES
            //   DE 100e EN 1000e ET DE 1 / 4h EN 1 / 2h

            int[,,,] KECR = new int[NBSTA + 1, 3 + 1, 48 + 1, NIND + 1];
            int[,,,] KECRIDF = new int[NBSTAIDF + 1, 3 + 1, 48 + 1, NIND + 1];
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
            for (int IND = 1; IND <= NIND; IND++)
            {
                POIDS[IND] *= 10;
                RIEN[IND] = 0;
            }
            for (int IND = 1; IND <= NIND; IND++) bwNat.Write(Convert.ToInt16(POIDS[IND]));
            for (int IND = 1; IND <= NIND; IND++) bwNat.Write(Convert.ToInt16(RIEN[IND]));

            // BOUCLE STATIONS
            int IDF = 0;
            for (int IS = 1; IS <= NBSTA; IS++)
            {
                int IFO = ISTA[IS];

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
                            KECR[IS, IU, IQ4, II] = KHI2[IS, IU, IH, II] + KHI2[IS, IU, IH + 1, II];      //  [STATIONS, LV/Sa/Di, QH, INDIVS]

                            KECR[IS, IU, IQ4, II] *= 5;

                            if (KECR[IS, IU, IQ4, II] < 0) Console.WriteLine($"{IS}, {IU}, {IQ4}");

                            bwNat.Write(Convert.ToInt16(KECR[IS, IU, IQ4, II]));
                        }

                        // ECRITURE FICHIER IDF
                        if (IFO == 1)
                        {
                            IDF++;
                            for (int IN = 1; IN <= NIND; IN++)
                            {
                                if (FILT[IN] == 0)
                                    KECRIDF[IS, IU, IQ4, IN] = 0;
                                else
                                    KECRIDF[IS, IU, IQ4, IN] = KECR[IS, IU, IQ4, IN];

                                bwIdf.Write(Convert.ToInt16(KECRIDF[IDF, IU, IQ4, IN]));
                            }
                        }
                    }
                }
            }

            BSUP.POIDS = POIDS;
            BSUP.RIEN = RIEN;
            BSUP.KECR = KECR;
            BSUP.KECRIDF = KECRIDF;

            for (int IND = 1; IND <= NIND; IND++) bwSup.Write(Convert.ToInt16(POIDS[IND]));
            for (int IND = 1; IND <= NIND; IND++) bwSup.Write(Convert.ToInt16(RIEN[IND]));

            for (int IS = 1; IS <= NBSTA; IS++)
            {
                // BOUCLE UNIVERS
                for (int IU = 1; IU <= 3; IU++)
                {
                    for (int IQ = 1; IQ <= 48; IQ ++)
                    {
                        for (int II = 1; II <= NIND; II++)
                        {
                            bwSup.Write(Convert.ToInt16(KECR[IS, IU, IQ, II]));
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
                            bwSup.Write(Convert.ToInt16(KECRIDF[IS, IU, IQ, II]));
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
            int[,,] VSOR2 = new int[NBJOUR + 1, 6, NBSTA + 1];
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
                                int[] bits = new int[7];
                                for (int b = 1; b <= 6; b++)
                                    bits[b] = JN[I][IG].VSor[b, K];

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
                    ITAP[K] = ITAP[K] + FLAG[K, IG] * 10 * POIDS[IG];
                }
                swPenetr.WriteLine(strStations[K].PadLeft(49,' ') + "," + ((100d * ITAP[K]) / population).ToString("0.00"));
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
            int[,] TABCI = new int[N + 1, NIND + 1];
            int[] IPOPS = new int[N + 1];
            double[] COUV = new double[N + 1];
            int[,,] TABRES = new int[N + 1, NBSTATOTAL  + 1, NIND + 1];
            int[] IG = new int[N + 1];

            // OUVERTURE DES FICHIERS

            //      OPEN(7, FILE = '#OUTPUT#PANRA1#YEAR#.SUP',
            //     -FORM = 'UNFORMATTED', RECORDTYPE = 'FIXED', RECL = 2 * NIND)
            //      READ(7) IPO2
            //      READ(7) VECT

            for (int IND = 1; IND <= NIND; IND++)
            {
                IPOI[IND] = BSUP.POIDS[IND];
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
                            if (BSUP.KECR[IS, IU, ITR, IND] > 0)  // [NBSTA + 1, 3 + 1, 48 + 1, NIND + 1]
                            {
                                for (int IN = 1; IN <= N; IN++)
                                {
                                    if (TABCI[IN, IND] == 1) TABRES[IN, IS, IND] = 1;
                                    if (TABCI[IN, IND] == 1) TABRES[IN, NBSTATOTAL, IND] = 1;
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
                for (int I = 1; I < N; I++)
                {
                    for (int IND = 1; IND <= NIND; IND++)
                    {
                        if (TABRES[I, 1, IND] == 1) COUV[I] += IPOI[IND];
                    }
                    COUV[I] *= 100d / IPOPS[I];
                }
                swPenetr.WriteLine(stations[STA].PadLeft(49) + COUV[1].ToString("0.00").PadLeft(8) + " " + COUV[2].ToString("0.00").PadLeft(8) + " " + COUV[3].ToString("0.00").PadLeft(8));
            }


            for (int I = 1; I <= N; I++)
            {
                for (int IND = 1; IND <= NIND; IND++)
                {
                    if (TABRES[I, NBSTATOTAL, IND] == 1) COUV[I] += IPOI[IND];
                }
                COUV[I] *= 100d / IPOPS[I];
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
        public void CALDISTR(double ZR, double UR, double PO, double TAUX, out double[] DISTR, out double[] REP)
        {
            int NSEG = 100;
            int JMAX = JMAX = NSEG * 2;
            int JSUP = JMAX + 2;
            double ALPHA, BETA, X, V, Q, Y, BAB, LEBND, Z, U, P, TAU;
            double[] LOGS = new double[JMAX + 1];
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
            Q = 1d - P;
            if (TAU <= 1E-4)
            {
                TAU = 0;
                ALPHA = P * 1E4;
                BETA = Q * 1E4;
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
                    BUFS[J] = Math.Exp((ALPHA - 1) * LOGS[J] + (BETA - 1) * LOGS[JSUP - J]);
                // ASTUCE POUR LES EXTREMITES
                LEBND = LOGS[2];
                X = 1d / (6 * (NSEG + 1));
                Y = 1d - (BETA - 1d) / JSUP;
                if (Y < 0.8) Y = 0.8;
                if (Y > 1.2) Y = 1.2;
                DISTR[0] = Y * Math.Exp(ALPHA * LEBND) / ALPHA;
                Y = 1d - (ALPHA - 1d) / JSUP;

                if (Y < 0.8) Y = 0.8;
                if (Y > 1.2) Y = 1.2;
                DISTR[NSEG] = Y * Math.Exp(BETA * LEBND) / BETA;
                // SEGMENTS INTERMEDIAIRES
                for (int I = 1; I <= NSEG - 1; I++)
                    DISTR[I] = (BUFS[2 * I] + BUFS[2 + 2 * I] + 4 * BUFS[1 + 2 * I]) * X;
                // NORMALISATION DE DISTR
                BAB = 0d;
                for (int I = 0; I <= NSEG; I++)
                    BAB = BAB + DISTR[I];
                for (int I = 0; I <= NSEG; I++)
                    DISTR[I] = DISTR[I] / BAB;
            }

            // CORRECTION DES Z ET U
            V = 1d - Z - U;
            for (int I = 0; I <= NSEG; I++)
                DISTR[I] = DISTR[I] * V;
            DISTR[0] = DISTR[0] + Z;
            DISTR[NSEG] = DISTR[NSEG] + U;
            REP[0] = DISTR[0];
            for (int I = 1; I <= NSEG; I++)
                REP[I] = REP[I - 1] + DISTR[I];
        }
    }

    public struct BSupport
    {
        public List<int> POIDS;
        public int[] RIEN;
        public int[,,,] KECR;
        public int[,,,] KECRIDF;
    }
}

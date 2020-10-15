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
            double REAL, NOTI, SGN, SGN0;
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
                                                SGN= Z < 0?-1d:1d;

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


        private void transp08()
        {
//            C PANEL RADIO 08 MEDIAMETRIE(nouveau format)
//C TRANSFORMATION DES PROBABILITES INDIVIDUELLES
//C        DE 100e EN 1000e ET DE 1 / 4h EN 1 / 2h

//   INCLUDE 'FSUBLIB.FI'
//      PARAMETER(NIND =#NB_INDIV#)
      
//C Le nombre de station correspond au nombre de stations(#NB_STA_HAB_NOTO_TOTAL#) - #NB_STA_TOTAL_ONLY# pour Total Radio (et Total TV)
//C Il ne nous reste plus que #NB_STA_HAB_NOTO,0# stations puisqu'on a supprimé SUD RADIO
//      PARAMETER(NBSTA =#NB_STA_HAB_NOTO,0#)
      
//      PARAMETER(NBSTAIF =#NB_STA_IDF#)
            
//      INTEGER * 1 KHI2(NIND, 96), ISTA(NBSTA)
//      INTEGER * 2 KECR(NIND), FILT(NIND), POIDS(NIND), RIEN(NIND)

//#STA_IDF_LIST_COMMENT#

//      DATA ISTA /#STA_IDF_LIST_NO_SUDRAD_MASK#/

//C                  INITIALISATIONS
//      OPEN(17, FILE = '#OUTPUT#FILTREIF',
//     -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
//      READ(17) FILT
//      CLOSE(17)
//      OPEN(17, FILE = '#OUTPUT#POIDSEGS',
//     -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
//      READ(17) POIDS
//      CLOSE(17)

//      OPEN(13, FILE = '#OUTPUT#PANECR#YEAR#',
//     -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
//      OPEN(14, FILE = '#OUTPUT#PANRA1#YEAR#.NAT',
//     -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
//      OPEN(15, FILE = '#OUTPUT#PANRA1#YEAR#.IDF',
//     -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
//      OPEN(16, FILE = '#OUTPUT#PANRA1#YEAR#.SUP',
//     -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')


//C                  COEFF.ET DEPARTEMENT
//      DO IND = 1, NIND
//        IPOI = POIDS(IND)

//    IPOI = IPOI * 10

//    POIDS(IND) = IPOI

//    RIEN(IND) = 0
//      ENDDO
//      WRITE(14) POIDS
//      WRITE(14) RIEN
//C                  BOUCLE STATIONS

//      DO IS = 1, NBSTA
//        IFO = ISTA(IS)

//    print *, 'Traitement station', IS
//C                  BOUCLE UNIVERS

//    DO IU = 1, 3
//C                  BOUCLE 1 / 4h - LECTURE
//          DO IQ = 1, 96

//        READ(13)(KHI2(IND, IQ), IND = 1, NIND)

//      ENDDO
//C                  BOUCLE 1 / 4h - PASSAGE A LA 1 / 2h
//          DO IQ = 1, 96, 2

//        IH = IQ + 20

//        IF(IH.GT.96) IH = IH - 96
//C                  BOUCLE INDIVIDUS
//            DO II = 1, NIND

//          KECR(II) = KHI2(II, IH)

//          KECR(II) = KECR(II) + KHI2(II, IH + 1)

//            KECR(II) = KECR(II) * 5

//          IF(KECR(II).LT.0) print *, IS, IU, IQ

//        ENDDO
//C                  ECRITURE FICHIER TOTAL
//            WRITE(14) KECR
//C                  ECRITURE FICHIER IDF
//            IF(IFO.EQ.1) THEN

//          DO IN = 1, NIND

//            IF(FILT(IN).EQ.0) KECR(IN) = 0

//          ENDDO

//          WRITE(15) KECR

//        ENDIF

//      ENDDO
//        ENDDO
//      ENDDO
//      CLOSE(14)
//      CLOSE(15)


//      OPEN(14, FILE = '#OUTPUT#PANRA1#YEAR#.NAT',
//     -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')
//      OPEN(15, FILE = '#OUTPUT#PANRA1#YEAR#.IDF',
//     -RECORDTYPE = 'FIXED', FORM = 'UNFORMATTED')


//C     POIDS
//            READ(14) KECR

//        WRITE(16) KECR
//C     VECTEUR VIDE

//        READ(14) KECR

//        WRITE(16) KECR
//      DO IS = 1, NBSTA
//C                  BOUCLE UNIVERS
//C Le 144 dans la ligne suivante correspond à 3 univers * 48 demi - heures

//    DO IU = 1, 144

//        READ(14) KECR

//        WRITE(16) KECR

//    ENDDO
//      ENDDO
//      DO IS = 1, NBSTAIF
//C                  BOUCLE UNIVERS

//    DO IU = 1, 144

//        READ(15) KECR

//        WRITE(16) KECR

//    ENDDO
//      ENDDO
//      CLOSE(14)
//      CLOSE(15)
//      CLOSE(16)



//      STOP
//      END

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
    }

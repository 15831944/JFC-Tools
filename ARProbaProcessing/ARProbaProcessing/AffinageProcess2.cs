using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ARProbaProcessing
{
    public partial class AffinageProcess
    {
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

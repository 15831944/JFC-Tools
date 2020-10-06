using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ARProbaProcessing
{
    public class TestProcessor
    {
        private ARProba aRProba;

        private double[] _weight;
        private double _population;

        List<double[][]> _proba;

        public TestProcessor(ARProba aRProba)
        {
            this.aRProba = aRProba;
            string probaFile = aRProba.OutputPath + "PANRA1" + aRProba.YearName + ".SUP";

            //string probaFile = @"C:\Work\AR\Probabilisation\FR\Panel National\Panfra10-V1\Output\" + "PANRA1" + aRProba.YearName + ".SUP";

            var pfs = File.Open(probaFile, FileMode.Open, FileAccess.Read);
            BinaryReader reader = new BinaryReader(pfs);

            int indivCount = aRProba.IndivCount;
            _weight = new double[indivCount];

            _population = 0;

            // Read weight vector
            for (int i = 0; i < indivCount; i++)
            {
                double w = ((double)reader.ReadUInt16());
                _weight[i] = w;
                _population += w;
            }

            // Read empty vector
            for (int i = 0; i < indivCount; i++)
            {
                reader.ReadUInt16();
            }

            _proba = new List<double[][]>();

            while (pfs.Position < pfs.Length)
            {
                _proba.Add(ReadStationProba(reader, indivCount));
            }

            reader.Close();
            pfs.Close();
        }

        public const int ENTRY_COUNT = 24 * 2 * 3; // 24h * 1/2h * L-V S D
        public const int ENTRY_COUNT_L_V = 24 * 2;
        public const int ENTRY_COUNT_L_S = 24 * 2 * 2;

        private static double[][] ReadStationProba(BinaryReader reader, int indivCount)
        {
            double[][] res = new double[ENTRY_COUNT][];
            for (int entry = 0; entry < ENTRY_COUNT; entry++)
            {
                res[entry] = new double[indivCount];

                for (int i = 0; i < indivCount; i++)
                {
                    double w = ((double)reader.ReadUInt16()) / 1000;
                    res[entry][i] = w;
                }

            }

            return res;
        }

        public double[] ComputeWeeklyGlobalProba(int staIdx)
        {
            double[] res = new double[aRProba.IndivCount];

            double[][] data = _proba[staIdx];

            for (int i = 0; i < aRProba.IndivCount; i++)
            {
                double iproba = 1;
                // L-V
                for (int entry = 0 + (5 * 2); entry < ENTRY_COUNT_L_V; entry++)
                {
                    for (int j = 0; j < 5 * 3; j++)
                    {
                        iproba *= (1 - data[entry][i]);
                    }
                }

                // S and D
                for (int entry = ENTRY_COUNT_L_V + (5 * 2); entry < ENTRY_COUNT_L_S; entry++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        iproba *= (1 - data[entry][i]);
                    }
                }

                for (int entry = ENTRY_COUNT_L_S + (5 * 2); entry < ENTRY_COUNT; entry++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        iproba *= (1 - data[entry][i]);
                    }
                }

                res[i] = 1 - iproba;
            }
            return res;
        }

        public double ComputeWeeklyCouv(int staIdx)
        {
            var prob = ComputeWeeklyGlobalProba(staIdx);
            double sum = 0;

            for (int i = 0; i < aRProba.IndivCount; i++)
            {
                sum += prob[i] * _weight[i];
            }

            return sum / _population * 100;
        }

        public double GetCouvLV700730(int staIdx, int entry)
        {
            double sum = 0;

            double[][] data = _proba[staIdx];


            for (int i = 0; i < aRProba.IndivCount; i++)
            {
                //if (data[4][i] != 0)
                //    sum += _weight[i];
                sum += data[entry][i] * _weight[i];
            }

            //for (int i = 0; i < aRProba.IndivCount; i++)
            //{
            //    double iproba = 1;
            //    // L-V
            //    for (int entry = 0 + (5 * 2); entry < ENTRY_COUNT_L_V; entry++)
            //    {
            //        iproba *= (1 - data[entry][i]);
            //    }
            //}

            //return sum / _population * 100;

            return sum;
        }

        public double[] ComputeWeeklyCouv()
        {
            double[] res = new double[_proba.Count];

            for (int idx = 0; idx < _proba.Count; idx++)
            {
                res[idx] = ComputeWeeklyCouv(idx);
            }

            return res;
        }

    }
}

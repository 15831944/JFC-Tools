using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ARProbaProcessing
{
    public class F04Processor
    {
        private ARProba aRProba;

        public F04Processor(ARProba aRProba)
        {
            this.aRProba = aRProba;
        }

        public string BDEPath
        { get { return aRProba.InputPath + @"BDE\"; } }

        public string F04Path
        { get { return aRProba.InputPath + @"F04\"; } }

        public void ConvertBDEFiles()
        {
            if (!Directory.Exists(aRProba.InputPath + @"F04"))
                Directory.CreateDirectory(aRProba.InputPath + @"F04");


            //	Ouverture simultanee de tous les fichiers BDE des stations
            StreamReader[] bdeFiles = new StreamReader[aRProba.StationCount];
            for (int sta = 0; sta < aRProba.StationCount; sta++)
            {
                string fname = string.Format("{0}ST{1:00}.BDE", BDEPath, sta + 1);
                bdeFiles[sta] = File.OpenText(fname);
            }

            //	Fichier contenant les 260 1ers caracteres du fichier .F04
            //	Remarque : on peut remplacer les 260 1ers caracteres par n'importe
            //	quelle chaine de 260 caracteres.
            byte[] refF04buffer = File.ReadAllBytes(aRProba.ProgramPath + @"F04Ref\Fic260c.bin");

            //	Ouverture simultanee des fichiers des jours
            BinaryWriter[] f04Files = new BinaryWriter[aRProba.DayCount];
            for (int day = 0; day < aRProba.DayCount; day++)
            {
                string fname = string.Format("{0}JN{1:00}NI{2}.F04", F04Path, day + 1, aRProba.YearName);
                f04Files[day] = new BinaryWriter(File.Open(fname, FileMode.Create));
                
                //	Ecriture des 260 caracteres au debut des fichiers des jours
                f04Files[day].Write(refF04buffer);

            }


            short[] tab_qh = new short[96];

            for (int day = 0; day < aRProba.DayCount; day++)
            {
                for (int ind = 0; ind < aRProba.IndivCount; ind++)
                 {

                    if (ind == 337 || ind == 336)
                    {
                        bool Ok = true;

                        int ll = ind + 1;
                    }

                    for (int sta = 0; sta < aRProba.StationCount; sta++)
                    {

                    

                        string line = bdeFiles[sta].ReadLine();

                        short K101 = short.Parse(line.Substring(0,4));
                        short K102 = short.Parse(line.Substring(4, 4));
                        short Refe = short.Parse(line.Substring(8, 4));
                        short jour = short.Parse(line.Substring(12, 2));

                        int jourIdx = jour - 1;
                        var f04File = f04Files[jourIdx];

                        if (sta == 0)
                        {
                            //	Rangement des 3 1ers shorts pour l'individu
                            f04File.Write(Refe);
                            f04File.Write(K101);
                            f04File.Write(K102);
                        }

                        //for (int qh = 0; qh < 96; qh++) tab_qh[qh] = short.Parse(line.Substring(14 + qh, 1));
                        for (int qh = 0; qh < 96; qh++)
                        {
                            //tab_qh[qh] = short.Parse(line[14 + qh].ToString());
                            tab_qh[qh] = (short)(line[14 + qh] - '0');
                        }

                        UInt16[] tab = new UInt16[6];

                        //	Rangement des 16 qh sur les 16 bits d'un short
                        for (int i = 0; i < 6; i++)
                        {
                            for (int qh = 0; qh < 16; qh++)
                            {
                                tab[i] <<= 1;
                                tab[i] += (UInt16)tab_qh[(i * 16) + qh];
                            }

                            f04File.Write(tab[i]);
                        }
                    }
                }

                f04Files[day].Close();

                System.Console.Out.WriteLine(string.Format("Traitement jour {0:00}",day+1));
            }


            for (int sta = 0; sta < aRProba.StationCount; sta++)
            {
                bdeFiles[sta].Close();
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace PopCornToAtelierRadio
{
    public class NumberStudy
    {


        public  static void FillNumber(String _pathRadio, StudyRoot_Cartouche l_cartouche)
        {
            String m_nameCompteur = _pathRadio + "\\USER\\UFR02\\Compteur.etu"; //"C:\\ARTRADIO\\USER\\UFR02\\Compteur.etu";
            if (!File.Exists(m_nameCompteur))
            {
                m_nameCompteur = "Compteur.etu";
                if (!File.Exists(m_nameCompteur))
                {
                    using (FileStream stream = new FileStream(m_nameCompteur, FileMode.Create))
                    {
                        using (var l_writ = new BinaryWriter(stream))
                        {
                            l_writ.Write(1);
                            l_writ.Close();
                        }
                    }
                }

            }

            if (File.Exists(m_nameCompteur))
            {
                // Lecture n° étude disponible en cours
                using (var streamCpt = new FileStream(m_nameCompteur, FileMode.Open, FileAccess.Read))
                {
                    using (var readerCpt = new BinaryReader(streamCpt))
                    {
                        l_cartouche.Information.NumeroEtu = readerCpt.ReadInt32();
                    }
                }

                // Ecriture nouveau n° etude max Modif ALAIN
                using (FileStream stream = new FileStream(m_nameCompteur, FileMode.Create))
                {
                    using (var l_writ = new BinaryWriter(stream))
                    {
                        l_writ.Write(l_cartouche.Information.NumeroEtu + 1);
                        l_writ.Close();
                    }
                }
            }
            else
            {
                // Fichier compteur non trouvé --> Init à 0
                l_cartouche.Information.NumeroEtu = 0;
            }
        }

    }
}

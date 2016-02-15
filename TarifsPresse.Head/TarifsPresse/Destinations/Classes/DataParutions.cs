using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Windows.Forms;
using System.Globalization;

namespace TarifsPresse.Destination.Classes
{
	public class DataParutions : IData
	{
        public class Parution
        {
            public string m_SupportIdentifier;
            public uint m_CodeTarif;
            public DateTime m_DateParution;
            public uint m_NumeroParution;
            public string m_ThemeRedactionnel;
        }

        public List<Parution> m_Parutions { get; set; }

        public DataParutions(DateTime date)
		{
            m_Parutions = new List<Parution>();
		}

        public bool AddParutions(string supportIdentifier, uint codeTarif, DateTime dateParution, uint numeroParution, string themeRedactionnel)
        {
            var p = m_Parutions.Find(t => t.m_SupportIdentifier == supportIdentifier &&
                t.m_CodeTarif == codeTarif &&
                t.m_DateParution == dateParution &&
                t.m_NumeroParution == numeroParution);

            if (p != null)
            {
                if (p.m_ThemeRedactionnel.Length == 0 && themeRedactionnel.Length > 0)
                {
                    p.m_ThemeRedactionnel = themeRedactionnel;
                    return true;
                }
                //MessageBox.Show(String.Format("Le support {0} contient déjà le code tarif {1} avec la date {2} et le numero de parution {3} !", supportIdentifier, codeTarif, dateParution, numeroParution));
                return false;
            }
            m_Parutions.Add(new Parution()
            {
                m_SupportIdentifier = supportIdentifier,
                m_CodeTarif = codeTarif,
                m_DateParution = dateParution,
                m_NumeroParution = numeroParution,
                m_ThemeRedactionnel = themeRedactionnel
            });
            return true;
        }

        public static void write(string directory, DateTime _date, List<Parution> parutions)
		{
            parutions.Sort((p1, p2) =>
            {
                int comp = p1.m_SupportIdentifier.CompareTo(p2.m_SupportIdentifier);
                if (comp != 0)
                    return comp;
                comp = (int)p1.m_CodeTarif - (int)p2.m_CodeTarif;
                if (comp != 0)
                    return comp;
                comp = p1.m_DateParution.CompareTo(p2.m_DateParution);
                if (comp != 0)
                    return comp;
                return (int)p1.m_NumeroParution - (int)p2.m_NumeroParution;
            });

            var dateStr = _date.ToString("ddMMyy");
            var path = directory + @"\PP" + dateStr + ".dat";

			// on cree le fichier et on serialise
			using (StreamWriter sw = new StreamWriter(path, /*append*/false, Encoding.GetEncoding("iso-8859-1")))
			{
				// 1ere ligne : entête -> date
                sw.Write("D" + _date.ToString("ddMMyyyy"));

				//les données
                foreach (Parution _parution in parutions)
				{
                    // 'P' for Parution
                    // Identifiant du support, 6 chars, space padded, left aligned (-)
                    // CodeTarif, 4 chars, 0 padded, right aligned, (doit être >= 0)
                    // Date de parution, 8 chars, (AAAAMMJJ)
                    // Numéro de parution, 5 chars, 0 padded, right aligned, , (doit être >= 0)
                    // Thème rédactionnel, 50 chars, space padded, left aligned (-)

                    int tr = _parution.m_ThemeRedactionnel.Length > 50 ? _parution.m_ThemeRedactionnel.Length : 50;
                    sw.Write("\r\nP{0,-6}{1,4}{2,8}{3,5}{4,-" + tr.ToString() + "}",
                        _parution.m_SupportIdentifier,
                        _parution.m_CodeTarif.ToString("D4"),
                        _parution.m_DateParution.ToString("yyyyMMdd"),
                        _parution.m_NumeroParution.ToString("D5"),
                        _parution.m_ThemeRedactionnel);
				}
			}
		}

        public bool read(FileInfo file, ProgressBar progressCtrl, DateTime _date)
        {
            m_Parutions.Clear();

            // on lis le fichier et on déserialise
            using (StreamReader sr = new StreamReader(file.OpenRead(), Encoding.GetEncoding("iso-8859-1")))
            {
                // 1ere ligne : entête -> date
                DateTime parsedDate;
                string date = sr.ReadLine();
                if (!date.StartsWith("D") ||
                    !DateTime.TryParseExact(date.TrimStart('D'), "ddMMyyyy", null, DateTimeStyles.None, out parsedDate) ||
                    parsedDate > _date)
                    return false;

                bool sansDoublons = true;

                //les données
                string[] lines = sr.ReadToEnd().Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
                progressCtrl.Minimum = 0;
                progressCtrl.Maximum = lines.Length;
                progressCtrl.Step = 1;

                foreach (string line in lines)
                {
                    // 'P' for Parution
                    // Identifiant du support, 6 chars, space padded, left aligned (-)
                    // CodeTarif, 4 chars, 0 padded, right aligned, (doit être >= 0)
                    // Date de parution, 8 chars, (AAAAMMJJ)
                    // Numéro de parution, 5 chars, 0 padded, right aligned, , (doit être >= 0)
                    // Thème rédactionnel, 50 chars, space padded, left aligned (-)

                    uint codeTarif, numeroParution;
                    DateTime _date2;
                    if ((line.Length < 1 + 6 + 4 + 8 + 5 + 50) ||
                        !line.StartsWith("P") ||
                        !uint.TryParse(line.Substring(7, 4), out codeTarif) ||
                        !DateTime.TryParseExact(line.Substring(11, 8), "yyyyMMdd", null, DateTimeStyles.None, out _date2) ||
                        !uint.TryParse(line.Substring(19, 5), out numeroParution))
                        return false;
                    sansDoublons &= AddParutions(line.Substring(1, 6).Trim(), codeTarif, _date2, numeroParution, line.Substring(24).Trim());
                    progressCtrl.Increment(1);
                }

                if (!sansDoublons)
                    MessageBox.Show("Attention, les parutions contiennent des doublons ! Ils sont ignorés.");
            }
            return true;
        }
    }
}

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
    public class DataVersions : IData
	{
        public class Version
        {
            public string m_SupportIdentifier;
            public uint m_CodeTarif;
            public DateTime m_Date;
        }

        public List<Version> m_versions { get; set; }

        public DataVersions(DateTime date)
		{
	         m_versions = new List<Version>();
		}

        private bool InternalAddVersions(string supportIdentifier, uint codeTarif, DateTime date)
        {
            if (m_versions.Exists(t => t.m_SupportIdentifier == supportIdentifier &&
                t.m_CodeTarif == codeTarif &&
                t.m_Date == date))
            {
                MessageBox.Show(String.Format("Le support {0} contient déjà la version de tarif {1} !", supportIdentifier, codeTarif));
                return false;
            }
            else
            {
                m_versions.Add(new Version()
                {
                    m_SupportIdentifier = supportIdentifier,
                    m_CodeTarif = codeTarif,
                    m_Date = date
                });
                return true;
            }
        }

        public static void write(string directory, DateTime _date, List<Version> versions)
		{
            versions.Sort((t1, t2) =>
            {
                int comp = t1.m_SupportIdentifier.CompareTo(t2.m_SupportIdentifier);
                if (comp != 0)
                    return comp;
                return t1.m_Date.CompareTo(t2.m_Date);
            });

            var dateStr = _date.ToString("ddMMyy");
            var path = directory + @"\PV" + dateStr + ".dat";

			// on cree le fichier et on serialise
			using (StreamWriter sw = new StreamWriter(path, /*append*/false, Encoding.GetEncoding("iso-8859-1")))
			{
				// 1ere ligne : entête -> date
                sw.Write("D" + _date.ToString("ddMMyyyy"));

				//les données
                foreach (Version version in versions)
				{
                    // 'V' for Versions
                    // Identifiant du support, 6 chars, space padded, left aligned (-)
                    // Date d’application, 8 chars, (AAAAMMJJ)
                    // CodeTarif, 4 chars, 0 padded, right aligned, (doit être >= 0)

                    sw.Write("\r\nV{0,-6}{1,8}{2,4}",
                        version.m_SupportIdentifier,
                        version.m_Date.ToString("yyyyMMdd"),
                        version.m_CodeTarif.ToString("D4"));
				}
			}
		}

        public bool read(FileInfo file, ProgressBar progressCtrl, DateTime _date)
        {
            m_versions.Clear();

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

                //les données
                string[] lines = sr.ReadToEnd().Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
                progressCtrl.Minimum = 0;
                progressCtrl.Maximum = lines.Length;
                progressCtrl.Step = 1;

                foreach (string line in lines)
                {
                    // 'V' for Versions
                    // Identifiant du support, 6 chars, space padded, left aligned (-)
                    // Date d’application, 8 chars, (AAAAMMJJ)
                    // CodeTarif, 4 chars, 0 padded, right aligned, (doit être >= 0)

                    uint codeTarif;
                    DateTime _date2;
                    if ((line.Length != 1 + 6 + 8 + 4) ||
                        !line.StartsWith("V") ||
                        !uint.TryParse(line.Substring(15, 4), out codeTarif) ||
                        !DateTime.TryParseExact(line.Substring(7, 8), "yyyyMMdd", null, DateTimeStyles.None, out _date2) ||
                        !InternalAddVersions(line.Substring(1, 6).Trim(), codeTarif, _date2))
                        return false;
                    progressCtrl.Increment(1);
                }
            }
            return true;
        }

	}
}

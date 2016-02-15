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
    public class DataSupports : IData
	{
        public class Support
        {
            public string m_Identifier;
            public string m_Libelle;
            public uint m_CodeTarif;

        }

		private List<Support> m_Supports { get; set; }
        private Dictionary<string, string> m_mappings { get; set; }

        public List<Support> Supports
        {
            get { return m_Supports; }
        }

        public Dictionary<string, string> Mappings
        {
            get { return m_mappings; }
        }

        
		public DataSupports(DateTime date)
		{
            m_Supports = new List<Support>();
            m_mappings = new Dictionary<string, string>();
		}

        private bool AddSupport(string identifier, uint codeTarif, string libelle)
        {
            if (m_Supports.Exists(s => s.m_Identifier == identifier && s.m_CodeTarif == codeTarif))
            {
                MessageBox.Show(String.Format("Le support {0} contient déjà le code tarif {1} !", identifier, codeTarif));
                return false;
            }
            m_Supports.Add(new Support() { m_Identifier = identifier, m_CodeTarif = codeTarif, m_Libelle = libelle });
            return true;
        }

        public static void write(string directory, DateTime date, List<Support> supports, Dictionary<string, string> mappings)
		{
            supports.Sort((s1, s2) => s1.m_Identifier.CompareTo(s2.m_Identifier) * 10000 + ((int)s1.m_CodeTarif - (int)s2.m_CodeTarif));

            var dateStr = date.ToString("ddMMyy");
            var path = directory + @"\PS" + dateStr + ".lib";

			// on cree le fichier et on serialise
			using (StreamWriter sw = new StreamWriter(path, /*append*/false, Encoding.GetEncoding("iso-8859-1")))
			{
				// 1ere ligne : entête -> date
                sw.Write("D" + date.ToString("ddMMyyyy"));

				//les données
                foreach (Support support in supports)
				{
                    // 'S' for Support
                    // Identifier, 6 chars, space padded, left aligned (-)
                    // CodeTarif, 4 chars, 0 padded, right aligned
                    // Libelle, 64 chars, space padded, left aligned (-)

                    sw.Write("\r\nS{0,-6}{1,4}{2,-64}",
                        support.m_Identifier,
                        support.m_CodeTarif.ToString("D4"),
                        support.m_Libelle);
				}
			}

            path += ".supports.map";
            // on cree le fichier et on serialise
            using (StreamWriter sw = new StreamWriter(path, /*append*/false, Encoding.GetEncoding("iso-8859-1")))
            {
                // 1ere ligne : entête -> date
                sw.Write("D" + date.ToString("ddMMyyyy"));

                //les données
                foreach (var mapping in mappings)
                    sw.Write("\r\n" + mapping.Key+'='+mapping.Value);
            }
        }

        public bool read(FileInfo file, ProgressBar progressCtrl, DateTime _date)
        {
            m_Supports.Clear();
            m_mappings.Clear();

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

				if (progressCtrl != null)
				{
					progressCtrl.Minimum = 0;
					progressCtrl.Maximum = lines.Length;
					progressCtrl.Step = 1;
				}

                foreach (string line in lines)
                {
                    // 'S' for Support
                    // Identifier, 6 chars, space padded, left aligned (-)
                    // CodeTarif, 4 chars, 0 padded, right aligned
                    // Libelle, 64 chars, space padded, left aligned (-)

                    uint codeTarif;
                    if ((line.Length != 1 + 6 + 4 + 64) ||
                        !line.StartsWith("S") ||
                        !uint.TryParse(line.Substring(7, 4), out codeTarif) ||
                        !AddSupport(line.Substring(1, 6).Trim(), codeTarif, line.Substring(11, 64).Trim()))
                        return false;
					if (progressCtrl != null) progressCtrl.Increment(1);
                }
            }

            var map = new FileInfo(file.FullName + ".supports.map");
            if (!map.Exists)
                return true;

            // on lis le fichier et on déserialise
            using (StreamReader sr = new StreamReader(map.OpenRead(), Encoding.GetEncoding("iso-8859-1")))
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
                foreach (string line in lines)
                {
                    var entry = line.Split('=');
                    m_mappings.Add(entry[0], entry[1]);
                }
            }
            return true;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using TarifsPresse_Codipress.Fenetres;

namespace TarifsPresse_Codipress
{
	public class DataTarifs
	{
		enum ColorsLibelles
		{
			Quadri = 1,
			Noir = 2,
			Bichro = 3,
			Trichro = 4
		}

		public class Tarif
		{
			public string m_SupportIdentifier;
			public uint m_CodeTarif;
			public uint m_FormatIdentifier;
			public uint m_ColorCode;
			public double m_Tarif;
		}

		public List<Tarif> m_tarifs { get; set; }

		private Dictionary<uint, string> m_formats { get; set; }
		private Dictionary<uint, string> m_colors { get; set; }
		public Dictionary<uint, string> Colors
		{
			get { return m_colors; }
		}
		public Dictionary<uint, string> Formats
		{
			get { return m_formats; }
		}

		private Dictionary<string, string> m_formatsMappings { get; set; }
		private Dictionary<string, string> m_colorMappings { get; set; }
		public Dictionary<string, string> ColorCodesMappings
		{
			get { return m_colorMappings; }
		}
		public Dictionary<string, string> FormatsMappings
		{
			get { return m_formatsMappings; }
		}

		public List<string> m_formatsIgnored;

		public DataTarifs()
		{
			m_tarifs = new List<Tarif>();
			m_formats = new Dictionary<uint, string>();
			m_colors = new Dictionary<uint, string>();
			m_colors.Add(1, "Quadri");
			m_colors.Add(2, "Noir");
			m_colors.Add(3, "Bichro");
			m_colors.Add(4, "Trichro");

			m_colorMappings = new Dictionary<string, string>();
			m_formatsMappings = new Dictionary<string, string>();
			m_formatsIgnored = new List<string>();
		}

		private bool InternalAddTarifs(string supportIdentifier, uint codeTarif, uint formatIdentifier, string formatLibelle, uint colorCode, double tarif)
		{
			if (m_tarifs.Exists(t => t.m_SupportIdentifier == supportIdentifier &&
				t.m_CodeTarif == codeTarif && t.m_ColorCode == colorCode &&
				t.m_FormatIdentifier == formatIdentifier))
			{
				MessageBox.Show(String.Format("Le support {0} contient déjà le code tarif {1} !", supportIdentifier, codeTarif));
				return false;
			}
			m_tarifs.Add(new Tarif()
			{
				m_SupportIdentifier = supportIdentifier,
				m_CodeTarif = codeTarif,
				m_FormatIdentifier = formatIdentifier,
				m_ColorCode = colorCode,
				m_Tarif = tarif
			});

			if (!m_formats.ContainsKey(formatIdentifier))
				m_formats.Add(formatIdentifier, formatLibelle);

			return true;
		}

		public static void write(string directory, DateTime _date, List<Tarif> tarifs, Dictionary<uint, string> formats,
			Dictionary<string, string> formatsMappings, Dictionary<string, string> colorMappings, List<string> formatsIgnored)
		{
			tarifs.Sort((t1, t2) =>
			{
				int comp = t1.m_SupportIdentifier.CompareTo(t2.m_SupportIdentifier) * 10000 + ((int)t1.m_CodeTarif - (int)t2.m_CodeTarif);
				if (comp != 0)
					return comp;
				return t1.m_FormatIdentifier.CompareTo(t2.m_FormatIdentifier);
			});

			var dateStr = _date.ToString("ddMMyy");
			var path = directory + @"\PT" + dateStr + ".lib";

			// on cree le fichier et on serialise
			using (StreamWriter sw = new StreamWriter(path, /*append*/false, Encoding.GetEncoding("iso-8859-1")))
			{
				// 1ere ligne : entête -> date
				sw.Write("D" + _date.ToString("ddMMyyyy"));

				//les données
				foreach (Tarif tarif in tarifs)
				{
					// 'T' for Tarifs
					// Identifiant du support, 6 chars, space padded, left aligned (-)
					// CodeTarif, 4 chars, 0 padded, right aligned, (doit être >= 0)
					// Identifiant du format, 10 chars, space padded, left aligned (-)
					// Libellé du format, 64 chars, space padded, left aligned (-)
					// Code couleur, 1 char	Numérique, (prends les valeurs 2 (noir), 3 (bichromie), 4 (trichromie) ou 1 (Quadrichromie))
					// Tarif brut en euros 8 chars Numérique flottant, 0 padded, right aligned, (doit être >= 0)

					string tarifstr;
					if (tarif.m_Tarif == Math.Truncate(tarif.m_Tarif))
						tarifstr = ((uint)tarif.m_Tarif).ToString("D").PadLeft(8, '0');
					else
						tarifstr = tarif.m_Tarif.ToString("F3", CultureInfo.InvariantCulture).TrimEnd('0', '.').PadLeft(8, '0');

					sw.Write("\r\nT{0,-6}{1,4}{2,10}{3,-64}{4,1}{5,8}",
						tarif.m_SupportIdentifier,
						tarif.m_CodeTarif.ToString("D4"),
						tarif.m_FormatIdentifier.ToString("D10"),
						formats[tarif.m_FormatIdentifier],
						tarif.m_ColorCode.ToString("D1"),
						tarifstr);
				}
			}


			var _path = path + ".formats.map";
			// on cree le fichier et on serialise
			using (StreamWriter sw = new StreamWriter(_path, /*append*/false, Encoding.GetEncoding("iso-8859-1")))
			{
				// 1ere ligne : entête -> date
				sw.Write("D" + _date.ToString("ddMMyyyy"));

				//les données
				foreach (var mapping in formatsMappings)
					sw.Write("\r\n" + mapping.Key + '=' + mapping.Value);
			}

			_path = path + ".colorCodes.map";
			// on cree le fichier et on serialise
			using (StreamWriter sw = new StreamWriter(_path, /*append*/false, Encoding.GetEncoding("iso-8859-1")))
			{
				// 1ere ligne : entête -> date
				sw.Write("D" + _date.ToString("ddMMyyyy"));

				//les données
				foreach (var mapping in colorMappings)
					sw.Write("\r\n" + mapping.Key + '=' + mapping.Value);
			}

			_path = path + ".formatsIgnored.list";
			// on cree le fichier et on serialise
			using (StreamWriter sw = new StreamWriter(_path, /*append*/false, Encoding.GetEncoding("iso-8859-1")))
			{
				// 1ere ligne : entête -> date
				sw.Write("D" + _date.ToString("ddMMyyyy"));

				//les données
				foreach (var format in formatsIgnored)
					sw.Write("\r\n" + format);
			}

		}

		public bool read(FileInfo file, MainWindow window, CancellationToken token, DateTime _date)
		{
			m_tarifs.Clear();
			m_formatsMappings.Clear();
			m_colorMappings.Clear();

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
				window.ProgressBarMax = lines.Length;
				window.ProgressBarValue = 0.0;


				foreach (string line in lines)
				{
					if (token.IsCancellationRequested)
						return false;

					// 'T' for Tarifs
					// Identifiant du support, 6 chars, space padded, left aligned (-)
					// CodeTarif, 4 chars, 0 padded, right aligned, (doit être >= 0)
					// Identifiant du format, 10 chars, space padded, left aligned (-)
					// Libellé du format, 64 chars, space padded, left aligned (-)
					// Code couleur, 1 char	Numérique, (prends les valeurs 1 (noir), 2 (bichromie), 3 (trichromie) ou 4 (Quadrichromie))
					// Tarif brut en euros 8 chars Numérique flottant, 0 padded, right aligned, (doit être >= 0)

					uint codeTarif, colorCode, codeFormat;
					double tarif;
					if ((line.Length != 1 + 6 + 4 + 10 + 64 + 1 + 8) ||
						!line.StartsWith("T") ||
						!uint.TryParse(line.Substring(7, 4), out codeTarif) ||
						!uint.TryParse(line.Substring(11, 10), out codeFormat) ||
						!uint.TryParse(line.Substring(85, 1), out colorCode) ||
						!double.TryParse(line.Substring(86, 8), NumberStyles.AllowDecimalPoint, CultureInfo.InvariantCulture, out tarif) ||
						!InternalAddTarifs(line.Substring(1, 6).Trim(), codeTarif, codeFormat, line.Substring(21, 64).Trim(), colorCode, tarif))
						return false;
					window.ProgressBarValue += 1.0;
				}
			}

			var fmap = new FileInfo(file.FullName + ".formats.map");
			if (fmap.Exists)
			{
				// on lis le fichier et on déserialise
				using (StreamReader sr = new StreamReader(fmap.OpenRead(), Encoding.GetEncoding("iso-8859-1")))
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
						m_formatsMappings.Add(entry[0], entry[1]);
					}
				}
			}

			var cmap = new FileInfo(file.FullName + ".colorCodes.map");
			if (cmap.Exists)
			{
				// on lis le fichier et on déserialise
				using (StreamReader sr = new StreamReader(cmap.OpenRead(), Encoding.GetEncoding("iso-8859-1")))
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
						m_colorMappings.Add(entry[0], entry[1]);
					}
				}
			}

			var fimap = new FileInfo(file.FullName + ".formatsIgnored.list");
			if (fimap.Exists)
			{
				// on lis le fichier et on déserialise
				using (StreamReader sr = new StreamReader(fimap.OpenRead(), Encoding.GetEncoding("iso-8859-1")))
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
						m_formatsIgnored.Add(line);
					}
				}
			}

			return true;
		}

	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TarifsPresse.Destination.Classes;
using System.Windows.Forms;
using IMP_CODI_PRESS_TARIFS_LOG;
using TarifsPresse.DataSet1TableAdapters;
using System.Data;

namespace TarifsPresse.Destinations.Classes
{
	public class Data
	{
		public class Tarif
		{
			public uint m_FormatIdentifier;
			public uint m_ColorCode;
			public double m_Tarif;
		}

		public class Tarifs
		{
			public uint m_CodeTarif;
			public DateTime m_Date;
			public Dictionary<KeyValuePair<uint, uint>, Tarif> m_tarifs = new Dictionary<KeyValuePair<uint, uint>, Tarif>();
			public List<Parution> m_Parutions = new List<Parution>();
		}

		public class Parution
		{
			public DateTime m_DateParution;
			public uint m_NumeroParution;
			public string m_ThemeRedactionnel;

			public static bool operator ==(Parution p1, Parution p2)
			{
				return p1.m_DateParution == p2.m_DateParution &&
					p1.m_NumeroParution == p2.m_NumeroParution;
			}

			public static bool operator !=(Parution p1, Parution p2)
			{
				return !(p1 == p2);
			}

			public override bool Equals(System.Object obj)
			{
				// If parameter is null return false.
				if (obj == null) return false;

				// If parameter cannot be cast to Parution return false.
				Parution p = obj as Parution;
				if ((System.Object)p == null) return false;

				// Return true if the fields match:
				return this == p;
			}

			public bool Equals(Parution p)
			{
				// If parameter is null return false:
				if ((object)p == null) return false;

				// Return true if the fields match:
				return this == p;
			}

			public override int GetHashCode()
			{
				return base.GetHashCode();
			}
		}

		public class Support
		{
			public string m_Identifier;
			public string m_Libelle;
			public Dictionary<uint/*code tarif*/, Tarifs> m_Tarifs;

			public Support(string identifier, string libelle)
			{
				m_Identifier = identifier;
				m_Libelle = libelle;
				m_Tarifs = new Dictionary<uint,Tarifs>();
			}

			public void AddTarif(DateTime date, uint formatIdentifier, uint color, double tarif)
			{
				uint codeTarif;

				Tarifs found;
				if ((found = m_Tarifs.Values.FirstOrDefault(v => v.m_Date == date)) == null)
				{
					if (m_Tarifs.Count > 0)
						codeTarif = m_Tarifs.Max(v1 => v1.Key) + 1;
					else
						codeTarif = 1;

					found = new Tarifs()
					{
						m_CodeTarif = codeTarif,
						m_Date = date
					};
					
					m_Tarifs.Add(codeTarif, found);
				}

				var key = new KeyValuePair<uint, uint>(formatIdentifier, color);
				if (!found.m_tarifs.ContainsKey(key))
				{
					found.m_tarifs.Add(key,
						new Tarif()
						{
							m_FormatIdentifier = formatIdentifier,
							m_ColorCode = color,
							m_Tarif = tarif
						});
				}
				else
				{

				}
			}
		}

		public Dictionary<string, Support> m_Data;

		public Dictionary<uint, string> m_formats;
		public Dictionary<uint, string> m_colors;

		public Dictionary<string, string> m_SupportMappings;
		public Dictionary<string, string> m_formatsMappings;
		public Dictionary<string, string> m_colorMappings;

		public List<string> m_formatsIgnored;

		public DateTime m_Date { get; set; }

		public bool m_IgnoreRubriques;
		public bool m_IgnoreTypePubliciteNotCOMMERCIALE;
		public bool m_IgnoreDiffusionNotNATIONAL;


		public Data(DateTime date, DataSupports supports, DataVersions versions, DataTarifs tarifs, DataParutions parutions)
		{
			m_Date = date;
			m_Data = new Dictionary<string,Support>();
			m_SupportMappings = new Dictionary<string, string>();

			m_IgnoreRubriques = true;
			m_IgnoreTypePubliciteNotCOMMERCIALE = true;
			m_IgnoreDiffusionNotNATIONAL = true;

			Import(supports, versions, tarifs, parutions);
		}

		private void Import(DataSupports supports, DataVersions versions, DataTarifs tarifs, DataParutions parutions)
		{
			supports.Supports.ForEach(support =>
			{
				if (!m_Data.ContainsKey(support.m_Identifier))
					m_Data.Add(support.m_Identifier, new Support(support.m_Identifier, support.m_Libelle));
			});

			m_SupportMappings = new Dictionary<string, string>(supports.Mappings);

			versions.m_versions.ForEach(version =>
			{
				Tarifs tarifVersion = new Tarifs();
				tarifVersion.m_CodeTarif = version.m_CodeTarif;
				tarifVersion.m_Date = version.m_Date;
				m_Data[version.m_SupportIdentifier].m_Tarifs[version.m_CodeTarif] = tarifVersion;

				var tarifs_found = tarifs.m_tarifs.Where(t =>t.m_SupportIdentifier == version.m_SupportIdentifier && t.m_CodeTarif == version.m_CodeTarif).ToList();
				tarifs_found.ForEach(t => {
					var key = new KeyValuePair<uint, uint>(t.m_FormatIdentifier, t.m_ColorCode);
					if (!tarifVersion.m_tarifs.ContainsKey(key))
					{
						Tarif tarif = new Tarif();
						tarif.m_FormatIdentifier = t.m_FormatIdentifier;
						tarif.m_ColorCode = t.m_ColorCode;
						tarif.m_Tarif = t.m_Tarif;
						tarifVersion.m_tarifs[key] = tarif;
					}
					else
					{

					}
				});
			});

			parutions.m_Parutions.ForEach(parution =>
			{
				var p = new Parution();
				p.m_DateParution = parution.m_DateParution;
				p.m_NumeroParution = parution.m_NumeroParution;
				p.m_ThemeRedactionnel = parution.m_ThemeRedactionnel;

				if (m_Data.ContainsKey(parution.m_SupportIdentifier) && m_Data[parution.m_SupportIdentifier].m_Tarifs.ContainsKey(parution.m_CodeTarif))
					m_Data[parution.m_SupportIdentifier].m_Tarifs[parution.m_CodeTarif].m_Parutions.Add(p);
				else
				{

				}
			});

			m_formats = new Dictionary<uint,string>(tarifs.Formats);
			m_colors = new Dictionary<uint,string>(tarifs.Colors);

			m_SupportMappings = new Dictionary<string,string>(supports.Mappings);
			m_formatsMappings = new Dictionary<string,string>(tarifs.FormatsMappings);
			m_colorMappings = new Dictionary<string,string>(tarifs.ColorCodesMappings);

			m_formatsIgnored = new List<string>(tarifs.m_formatsIgnored);
		}

		public void write(string directory)
		{
			//updateParutions();

			var supports = m_Data.SelectMany(kvp => {
				return kvp.Value.m_Tarifs.Select(kvpTarif => {
					return new DataSupports.Support()
					{
						m_Identifier = kvp.Value.m_Identifier,
						m_Libelle = kvp.Value.m_Libelle,
						m_CodeTarif = kvpTarif.Value.m_CodeTarif
					};
				});
			}).ToList();
			DataSupports.write(directory, m_Date, supports, m_SupportMappings);

			var versions = m_Data.SelectMany(kvp =>
			{
				return kvp.Value.m_Tarifs.Select(kvpVersionsTarif =>
				{
					return new DataVersions.Version()
					{
						m_SupportIdentifier = kvp.Value.m_Identifier,
						m_CodeTarif = kvpVersionsTarif.Value.m_CodeTarif,
						m_Date = kvpVersionsTarif.Value.m_Date
					};
				});
			}).ToList();
			DataVersions.write(directory, m_Date, versions);

			var tarifs = m_Data.SelectMany(kvp =>
			{
				return kvp.Value.m_Tarifs.SelectMany(kvpVersionsTarif =>
				{
					return kvpVersionsTarif.Value.m_tarifs.Select(kvpTarif =>
					{
						return new DataTarifs.Tarif()
						{
							m_SupportIdentifier = kvp.Value.m_Identifier,
							m_CodeTarif = kvpVersionsTarif.Value.m_CodeTarif,
							m_FormatIdentifier = kvpTarif.Value.m_FormatIdentifier,
							m_ColorCode = kvpTarif.Value.m_ColorCode,
							m_Tarif = kvpTarif.Value.m_Tarif
						};
					});
				});
			}).ToList();
			DataTarifs.write(directory, m_Date, tarifs, m_formats, m_formatsMappings, m_colorMappings, m_formatsIgnored);

			var parutions = m_Data.SelectMany(kvp =>
			{
				return kvp.Value.m_Tarifs.SelectMany(kvpVersionsTarif =>
				{
					return kvpVersionsTarif.Value.m_Parutions.Select(kvpParution =>
					{
						return new DataParutions.Parution()
						{
							m_SupportIdentifier = kvp.Value.m_Identifier,
							m_CodeTarif = kvpVersionsTarif.Value.m_CodeTarif,
							m_NumeroParution = kvpParution.m_NumeroParution,
							m_DateParution = kvpParution.m_DateParution,
							m_ThemeRedactionnel = kvpParution.m_ThemeRedactionnel
						};
					});
				});
			}).ToList();
			DataParutions.write(directory, m_Date, parutions);
		}

		enum EPeriodicity
		{
			_7_PAR_AN,
			BIMENSUEL,
			ANNUEL,
			SUPPLEMENT,
			SEMESTRIEL,
			_8_PAR_AN,
			HORS_SERIE,
			_5_PAR_AN,
			INDETERMINE,
			BIZARRE,
			MENSUEL,
			HEBDOMADAIRE,
			BIMESTRIEL,
			TRIMESTRIEL,
			PONCTUEL,
			QUOTIDIENNE
		}

		public void updateParutions()
		{
/*
			var adaptor = new NewspapersPeriodicityTableAdapter();
			var periodicities = adaptor.GetNewspapersPeriodicity();
			var newspapers = periodicities.Select(np => {
				
				EPeriodicity periodicity = EPeriodicity.INDETERMINE;
				if (np.PERIODICITY.CompareTo("QUOTIDIENNE") == 0)
					periodicity = EPeriodicity.QUOTIDIENNE;
				else if (np.PERIODICITY.CompareTo("HEBDOMADAIRE") == 0)
					periodicity = EPeriodicity.HEBDOMADAIRE;
				else if (np.PERIODICITY.CompareTo("BIMENSUEL") == 0)
					periodicity = EPeriodicity.BIMENSUEL;
				else if (np.PERIODICITY.CompareTo("MENSUEL") == 0)
					periodicity = EPeriodicity.MENSUEL;
				else if (np.PERIODICITY.CompareTo("BIMESTRIEL") == 0)
					periodicity = EPeriodicity.BIMESTRIEL;
				else if (np.PERIODICITY.CompareTo("TRIMESTRIEL") == 0)
					periodicity = EPeriodicity.TRIMESTRIEL;
				else if (np.PERIODICITY.CompareTo("SEMESTRIEL") == 0)
					periodicity = EPeriodicity.SEMESTRIEL;
				else if (np.PERIODICITY.CompareTo("ANNUEL") == 0)
					periodicity = EPeriodicity.ANNUEL;
				else if (np.PERIODICITY.CompareTo("5 PAR AN") == 0)
					periodicity = EPeriodicity._5_PAR_AN;
				else if (np.PERIODICITY.CompareTo("7 PAR AN") == 0)
					periodicity = EPeriodicity._7_PAR_AN;
				else if (np.PERIODICITY.CompareTo("8 PAR AN") == 0)
					periodicity = EPeriodicity._8_PAR_AN;
				else if (np.PERIODICITY.CompareTo("SUPPLEMENT") == 0)
					periodicity = EPeriodicity.SUPPLEMENT;
				else if (np.PERIODICITY.CompareTo("PONCTUEL") == 0)
					periodicity = EPeriodicity.PONCTUEL;
				else if (np.PERIODICITY.CompareTo("BIZARRE") == 0)
					periodicity = EPeriodicity.BIZARRE;
				
				return new
				{
					Newspaper = np.NEWSPAPER,
					Periodicity = periodicity
				};
			});
*/

			var notfound = new List<String>();

			var mappings_TNS = Form1.CSVFileToDataTable(@"C:\Work\JFC - DPS2\TarifsPresse.Head\TarifsPresse\bin\Debug\supportsTNS_Codipresse.csv");
			var columns = mappings_TNS.Columns.Cast<DataColumn>().Select(c => c.ColumnName).ToList();
			var mappings = mappings_TNS.Select().Select(r => columns.ToDictionary(col => col, col => r[col] as String)).ToList();
			var np_array = mappings.Select(r => r["supports codipresse"]).ToArray();
			var lev = new Levenshtein();
			lev.SetupTextSentences(np_array);

			m_Data.Values.ToList().ForEach(support =>
			{
				Dictionary<String, String> found = null;

				var result = lev.SearchSentenceRank(support.m_Libelle);
				var matches = result.OrderByDescending(res => res.Key).SelectMany(res => res.Value.Select(res2 => new { Result = res, Support = np_array[res2] })).ToList();
				if (matches[0].Result.Key == 1.0)
					found = mappings.Single(m => m["supports codipresse"] == matches[0].Support);
				else
				{
					var form = new FormMatchSupport(support.m_Libelle, matches.Select(m => m.Support).ToList());
					if (form.ShowDialog() == DialogResult.OK)
						found = mappings.Single(m => m["supports codipresse"] == form.selected);
					else
						notfound.Add(support.m_Libelle);
				}

				if (found != null)
				{
					Parution from;
					int to;//annee max
					var parutions = support.m_Tarifs.SelectMany(t => t.Value.m_Parutions).OrderBy(p => p.m_DateParution).ToList();
					if (parutions.Count == 0)
					{
						var ttt = support.m_Tarifs.OrderBy(v => v.Value.m_Date);
						to = ttt.Last().Value.m_Date.Year;
						from = new Parution() { m_NumeroParution = 1, m_DateParution = ttt.First().Value.m_Date, m_ThemeRedactionnel = "" };
						ttt.Last().Value.m_Parutions.Add(from);
					}
					else
					{
						from = parutions.Last();
						to = support.m_Tarifs.OrderBy(v => v.Value.m_Date).Last().Value.m_Date.Year;
						
					}
					//
					var next_date = from.m_DateParution;
					do
					{
						switch (found["periodicity"])
						{
							case "QUOTIDIENNE":  next_date = next_date.AddDays(1.0); break;
							case "HEBDOMADAIRE": next_date = next_date.AddDays(7.0); break;
							case "BIMENSUEL":    next_date = next_date.AddDays(next_date.AddMonths(1).Subtract(next_date).Days / 2.0); break;
							case "MENSUEL":      next_date = next_date.AddMonths(1); break;
							case "BIMESTRIEL":   next_date = next_date.AddMonths(2); break;
							case "TRIMESTRIEL":  next_date = next_date.AddMonths(3); break;
							case "SEMESTRIEL":   next_date = next_date.AddMonths(6); break;
							case "ANNUEL":       next_date = next_date.AddYears(1); break;
							case "5 PAR AN":
							case "7 PAR AN":
							case "8 PAR AN":
							case "SUPPLEMENT":
							case "PONCTUEL":
							case "BIZARRE":
							default:
								continue;
						}
						from = new Parution() { m_NumeroParution = from.m_NumeroParution + 1, m_DateParution = next_date, m_ThemeRedactionnel = "" };
						if (next_date.Year <= to)
							support.m_Tarifs.Last().Value.m_Parutions.Add(from);
					} while (next_date.Year <= to);
				}

				var tarifs = support.m_Tarifs.ToList();
				for (int i = 0; i < tarifs.Count; ++i)
				{
					if (i > 0)
					{
						// on décale les parutions appartenant à une version de tarif plus ancienne
						var parutionsToMove = tarifs[i].Value.m_Parutions.Where(t => t.m_DateParution <= tarifs[i].Value.m_Date).ToList();
						if (parutionsToMove.Count > 0)
						{
							parutionsToMove.ForEach(p =>
							{
								tarifs[i].Value.m_Parutions.Remove(p);
								if (!tarifs[i - 1].Value.m_Parutions.Contains(p))
									tarifs[i - 1].Value.m_Parutions.Add(p);
							});
							//Console.WriteLine(String.Format("{0} parutions déplacées avant pour i={1}", parutionsToMove.Count, i));

							//on repart de zéro
							i = -1;
							continue;
						}
					}

					if (i + 1 < tarifs.Count)
					{
						// on décale les parutions appartenant à une version de tarif plus récente
						var nextDate = tarifs[i + 1].Value.m_Date;
						var parutionsToMove = tarifs[i].Value.m_Parutions.Where(t => t.m_DateParution > nextDate).ToList();
						if (parutionsToMove.Count > 0)
						{
							parutionsToMove.ForEach(p =>
							{
								tarifs[i].Value.m_Parutions.Remove(p);
								if (!tarifs[i + 1].Value.m_Parutions.Contains(p))
									tarifs[i + 1].Value.m_Parutions.Add(p);
							});
							//Console.WriteLine(String.Format("{0} parutions déplacées après pour i={1}", parutionsToMove.Count, i));
						}
					}
				}
			});

			var count = notfound.Count;
			



// 				var parutions = support.m_Tarifs.SelectMany(v => v.Value.m_Parutions).OrderBy(p => p.m_DateParution).ToList();
// 
// 				Parution parution;
// 				if (parutions.Count > 0)
// 				{
// 					parution = parutions.Last();
// 
// 
// 					// analyse statistique des deltas des dates
// 					var stats = new SortedDictionary<TimeSpan, int>();
// 					var stats2 = new SortedDictionary<DayOfWeek, int>();
// 					stats2.Add(parutions[0].m_DateParution.DayOfWeek, 1);
// 					for (int index = 0; index < parutions.Count - 1; index++)
// 					{
// 						var delta = parutions[index + 1].m_DateParution - parutions[index].m_DateParution;
// 						if (stats.ContainsKey(delta))
// 							stats[delta]++;
// 						else
// 							stats.Add(delta, 1);
// 
// 						var dow = parutions[index + 1].m_DateParution.DayOfWeek;
// 						if (stats2.ContainsKey(dow))
// 							stats2[dow]++;
// 						else
// 							stats2.Add(dow, 1);
// 
// 					}
// 
// 					var max = stats.OrderByDescending(k => k.Value).First();
// 					var max2 = stats2.OrderByDescending(k => k.Value).First();
// 
// 
// 
// 
// 				}
// 				else
// 				{
// 
// 				}
// 

/*
				var result = lev.SearchSentenceRank(support.m_Libelle);
				var matches = result.OrderByDescending(res => res.Key).SelectMany(res => res.Value.Select(res2 => new { Result = res, Support = np_array[res2] })).ToList();
				if (matches[0].Result.Key == 1.0)
				{
					/ *trouvé* /
					var supp = mappings.Single(m => m["supports codipresse"] == matches[0].Support);
					var periodicity = supp["periodicity"];

					
					Parution parution;
					if (parutions.Count > 0)
						parution = parutions.Last();
					
/ *
					switch (periodicity)
					{
						case "QUOTIDIENNE": break;
						case "HEBDOMADAIRE": break;
						case "BIMENSUEL": break;
						case "MENSUEL": break;
						case "BIMESTRIEL": break;
						case "TRIMESTRIEL": break;
						case "SEMESTRIEL": break;
						case "ANNUEL": break;
						case "5 PAR AN": break;
						case "7 PAR AN": break;
						case "8 PAR AN": break;
						case "SUPPLEMENT": break;
						case "PONCTUEL": break;
						case "BIZARRE": break;
						default: break;
					}
* /
				}
				else
				{
// 					var form = new FormMatchSupport(support.m_Libelle, matches.Select(m => m.Support).ToList());
// 					var res = form.ShowDialog();
// 					if (res == DialogResult.OK)
// 					{
// 						var found = matches.First(m => m.Support == form.selected);
// 
// 					}
				}
*/
		}

		public Support FindSupport(string support_to_find, bool askToMap, bool map = false)
		{
			if (m_SupportMappings.ContainsKey(support_to_find))
			{
				if (m_Data.Values.FirstOrDefault(s => s.m_Libelle == m_SupportMappings[support_to_find]) != null)
				{
					support_to_find = m_SupportMappings[support_to_find];
					askToMap = false;
					map = false;
				}
			}

			if (!m_Data.Any(s => s.Value.m_Libelle == support_to_find) && (askToMap || map))
			{
				//on doit trouver le support correspondant
				if (!map)
					MessageBox.Show(string.Format("Le support suivant n'a pas été trouvé : {0}\r\nVeuillez choisir une correspondance ou créer un nouveau support.", support_to_find));
				var form = new FormSupports(this, support_to_find);
				if (form.ShowDialog() == DialogResult.OK)
					return form.m_MappedSupport;
				else
					return null;
			}
			return m_Data.Values.FirstOrDefault(s => s.m_Libelle == support_to_find);
		}

		public int FindColorCode(string colorCode_to_find, bool askToMap, bool map = false)
		{
			if (m_colorMappings.ContainsKey(colorCode_to_find))
			{
				colorCode_to_find = m_colorMappings[colorCode_to_find];
				askToMap = false;
			}
			var colorCodeExists = m_colors.ContainsValue(colorCode_to_find);
			if (!colorCodeExists && (askToMap || map))
			{
				//on doit trouver le color code correspondant
				if (!map)
					MessageBox.Show(string.Format("Le code couleur suivant n'a pas été trouvé : {0}\r\nVeuillez choisir une correspondance ou créer un nouveau code couleur.", colorCode_to_find));
				var form = new FormCouleurs(this, colorCode_to_find);
				if (form.ShowDialog() == DialogResult.OK)
					return (int)form.m_MappedCouleur;
				else
					return -1;
			}
			if (!colorCodeExists)
				return -1;
			return (int)m_colors.First(c => c.Value == colorCode_to_find).Key;
		}

		public bool AddSupport(string identifier, string libelle)
		{
			if (m_Data.ContainsKey(identifier))
			{
				MessageBox.Show(String.Format("Le support {0} existe déjà !", identifier));
				return false;
			}
			m_Data.Add(identifier, new Support(identifier, libelle));
			return true;
		}

		public void AddSupportMapping(string supportToMap, string mappedSupport)
		{
			if (!m_SupportMappings.ContainsKey(supportToMap))
				m_SupportMappings.Add(supportToMap, mappedSupport);
		}

		public void AddColorMapping(string couleurToMap, string mappedCouleur)
		{
			if (!m_colorMappings.ContainsKey(couleurToMap))
				m_colorMappings.Add(couleurToMap, mappedCouleur);
		}

		public void AddFormatMapping(string formatToMap, string mappedFormat)
		{
			if (!m_formatsMappings.ContainsKey(formatToMap))
				m_formatsMappings.Add(formatToMap, mappedFormat);
		}

		public bool AddFormat(uint identifier, string libelle)
		{
			if (m_formats.ContainsKey(identifier))
			{
				MessageBox.Show(String.Format("Le format {0} est déjà connu !", identifier));
				return false;
			}
			m_formats.Add(identifier, libelle);
			return true;
		}

		public string FindFormat(string format_to_find, bool askToMap, bool map, out bool ignored)
		{
			ignored = m_formatsIgnored.Contains(format_to_find);
			if (ignored)
				return null;

			if (m_formatsMappings.ContainsKey(format_to_find))
			{
				format_to_find = m_formatsMappings[format_to_find];
				askToMap = false;
			}
			var formatExists = m_formats.ContainsValue(format_to_find);
			if (!formatExists && (askToMap || map))
			{
				//on doit trouver le format correspondant
				if (!map)
					MessageBox.Show(string.Format("Le format suivant n'a pas été trouvé : {0}\r\nVeuillez choisir une correspondance ou créer un nouveau format.", format_to_find));
				var form = new FormFormats(this, format_to_find);
				if (form.ShowDialog() == DialogResult.OK)
				{
					ignored = m_formatsIgnored.Contains(format_to_find);
					if (ignored)
						return null;
					return form.m_MappedFormat;
				}
				else
					return null;
			}
			if (!formatExists)
				return null;
			return format_to_find;
		}

		public bool IsFormatIgnored(NewDataSetTARIF_INSERT insert)
		{
			if (m_IgnoreRubriques && insert.HasRubrique())
				return true;
			if (m_IgnoreDiffusionNotNATIONAL && insert.HasDiffusionNotNational())
				return true;
			if (m_IgnoreTypePubliciteNotCOMMERCIALE && insert.HasTypePubliciteNotCommerciale())
				return true;
			return false;
		}

		public bool FormatIdentifierExists(uint identifier)
		{
			return m_formats.ContainsKey(identifier);
		}

		public void AddFormatIgnored(string formatToIgnore)
		{
			if (!m_formatsIgnored.Contains(formatToIgnore))
				m_formatsIgnored.Add(formatToIgnore);
		}
	}
}

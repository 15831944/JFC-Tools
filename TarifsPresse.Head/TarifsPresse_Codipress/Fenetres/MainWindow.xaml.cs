using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;

namespace TarifsPresse_Codipress.Fenetres
{
	/// <summary>
	/// Logique d'interaction pour MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window, INotifyPropertyChanged
	{
		#region INotifyPropertyChanged
		public event PropertyChangedEventHandler PropertyChanged;

		// Create the OnPropertyChanged method to raise the event 
		protected void OnPropertyChanged(string name)
		{
			PropertyChangedEventHandler handler = PropertyChanged;
			if (handler != null)
			{
				handler(this, new PropertyChangedEventArgs(name));
			}
		}
		#endregion

		private DataBaseMBS m_data_MBS;
		private DataSupports m_DataSupports = new DataSupports();
		private DataVersions m_DataVersions = new DataVersions();
		private DataTarifs m_DataTarifs = new DataTarifs();
		private DataParutions m_DataParutions = new DataParutions();
		private CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
		private Dictionary<String, List<CodipressEntry> > m_CodipressEntriesBySupports;


		#region properties
		private double m_ProgressBarValue = 0;
		public double ProgressBarValue
		{
			get { return this.m_ProgressBarValue; }
			set {
				if (m_ProgressBarValue != value)
				{
					m_ProgressBarValue = value;
					OnPropertyChanged("ProgressBarValue");
				}
			}
		}

		private double m_ProgressBarMax = 100;
		public double ProgressBarMax
		{
			get { return this.m_ProgressBarMax; }
			set
			{
				if (m_ProgressBarMax != value)
				{
					m_ProgressBarMax = value;
					OnPropertyChanged("ProgressBarMax");
				}
			}
		}

		private String m_ProgressBarText;
		public String ProgressBarText
		{
			get { return this.m_ProgressBarText; }
			set
			{
				if (m_ProgressBarText != value)
				{
					m_ProgressBarText = value;
					OnPropertyChanged("ProgressBarText");
				}
			}
		}
		#endregion

		public MainWindow()
		{
			DataContext = this;
			InitializeComponent();
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			var dialog = new OpenFileDialog();
			dialog.Multiselect = false;
			dialog.CheckFileExists = true;
			dialog.CheckPathExists = true;
			dialog.DefaultExt = ".lib";
			dialog.Filter = "Supports Files (PS*.lib)|PS*.lib";
			dialog.Title = "Choisir le fichier des supports (PSxxxxxx.lib)";
			dialog.InitialDirectory = Directory.GetCurrentDirectory();

			if (dialog.ShowDialog(this).Value)
			{
				Cursor = Cursors.Wait;

				cancellationTokenSource = new CancellationTokenSource();
				var token = cancellationTokenSource.Token;

				MainProgress.Visibility = Visibility.Visible;
				MainProgressText.Visibility = Visibility.Visible;
				MainProgressCancel.Visibility = Visibility.Visible;

				Task.Factory.StartNew(() =>
				{
					var fileSupports = new FileInfo(dialog.FileName);
					var fileVersions = new FileInfo(fileSupports.DirectoryName + '\\' + fileSupports.Name.Replace("PS", "PV").Replace(".lib", ".dat"));
					var fileTarifs = new FileInfo(fileSupports.DirectoryName + '\\' + fileSupports.Name.Replace("PS", "PT"));
					var fileParutions = new FileInfo(fileSupports.DirectoryName + '\\' + fileSupports.Name.Replace("PS", "PP").Replace(".lib", ".dat"));

					ProgressBarText = "1 / 4 (" + fileSupports.Name + ")";
					if (!token.IsCancellationRequested && m_DataSupports.read(fileSupports, this, token, DateTime.Today))
					{
						ProgressBarText = "2 / 4 (" + fileVersions.Name + ")";
						if (!token.IsCancellationRequested && m_DataVersions.read(fileVersions, this, token, DateTime.Today))
						{
							ProgressBarText = "3 / 4 (" + fileTarifs.Name + ")";
							if (!token.IsCancellationRequested && m_DataTarifs.read(fileTarifs, this, token, DateTime.Today))
							{
								ProgressBarText = "4 / 4 (" + fileParutions.Name + ")";
								if (!token.IsCancellationRequested && m_DataParutions.read(fileParutions, this, token, DateTime.Today))
								{
									// chargement effectué, on produit la base de donnée de travail

									DateTime date;
									if (!DateTime.TryParseExact(fileSupports.Name.Substring(2, 6), "ddMMyy", CultureInfo.InvariantCulture, DateTimeStyles.None, out date))
										throw new InvalidDataException("date invalide : " + fileSupports.Name.Substring(2, 6));

									m_data_MBS = new DataBaseMBS(date, m_DataSupports, m_DataVersions, m_DataTarifs, m_DataParutions);
									m_DataSupports = null;
									m_DataVersions = null;
									m_DataTarifs = null;
									m_DataParutions = null;
									 
									return;
								}
								if (!token.IsCancellationRequested)
									throw new InvalidDataException("fichier invalide : " + fileParutions.Name);
							}
							if (!token.IsCancellationRequested)
								throw new InvalidDataException("fichier invalide : " + fileTarifs.Name);
						}
						if (!token.IsCancellationRequested)
							throw new InvalidDataException("fichier invalide : " + fileVersions.Name);
					}
					if (!token.IsCancellationRequested)
						throw new InvalidDataException("fichier invalide : " + fileSupports.Name);
					token.ThrowIfCancellationRequested();

				}, token)
				.ContinueWith((task) =>
				{
					Dispatcher.BeginInvoke(new Action(() =>
					{
						if (task.IsFaulted)
						{
							MessageBox.Show(task.Exception.InnerException.Message);
							Close();
							return;
						}
						else if (task.IsCanceled)
						{
							// on annule le chargement
							System.Diagnostics.Process.Start(Application.ResourceAssembly.Location);
							Close();
							return;
						}
						else
						{
							MainProgress.Visibility = Visibility.Hidden;
							MainProgressText.Visibility = Visibility.Hidden;
							MainProgressCancel.Visibility = Visibility.Hidden;
							// on continue le chargement
							ImportCodipressDataBase();
						}
						Cursor = Cursors.AppStarting;
					}));
				});
			}
			else
			{
				// Close the current application
				Dispatcher.BeginInvoke(new Action(() => { Close(); }));
			}
		}

		private void ImportCodipressDataBase()
		{
			var dialog = new OpenFileDialog();
			dialog.Multiselect = false;
			dialog.CheckFileExists = true;
			dialog.CheckPathExists = true;
			dialog.DefaultExt = "";
			dialog.Filter = "Codipress files (*.csv)|*.csv";
			dialog.Title = "Choisir le fichier csv Codipress à importer";
			dialog.InitialDirectory = Directory.GetCurrentDirectory();

			var res = dialog.ShowDialog(this);
			if (res != null && res.Value)
			{

				string file = dialog.FileName; // IMP_CODI_PRESS_TARIFS20131107070029.csv
				DataTable dataCodipress;
				if (!CSVFileToDataTable(file, out dataCodipress))
				{
					// Close the current application
					Close();
				}

				var filename = new FileInfo(file);
				m_data_MBS.m_Date = new DateTime(int.Parse(filename.Name.Substring(21, 4)), int.Parse(filename.Name.Substring(25, 2)), int.Parse(filename.Name.Substring(27, 2)));

				var CodipressEntries = dataCodipress.Select().Select(dr => new CodipressEntry(dr)).Where(e => !e.HasRubrique() && !e.HasTypePubliciteNotCommerciale() && !e.HasDiffusionNotNational()).ToList();
				m_CodipressEntriesBySupports = CodipressEntries.GroupBy(k => k.UniqueSupport()).ToDictionary(grp => grp.Key, grp => grp.ToList());

				//update counters supports, formats, colors
				UpdateCounters();
				Cursor = Cursors.Arrow;
			}
			else
			{
				// Close the current application
				Close();
			}
		}

		private void UpdateCounters()
		{
			var supports = m_CodipressEntriesBySupports.Keys.Except(m_data_MBS.m_supportsIgnored).ToList();
			text_supports.Text = string.Format("{0} / {1}",
				supports.Count - supports.Count(s => m_data_MBS.m_SupportMappings.ContainsKey(s) && m_data_MBS.m_Data.Any(sup => sup.Value.m_Libelle == m_data_MBS.m_SupportMappings[s])),
				supports.Count);

			var formats = m_CodipressEntriesBySupports.SelectMany(s => s.Value.Select(entry => entry.UniqueFormat())).Distinct().Except(m_data_MBS.m_formatsIgnored).ToList();
			text_formats.Text = string.Format("{0} / {1}",
				formats.Count - formats.Count(f => m_data_MBS.m_formatsMappings.ContainsKey(f) && m_data_MBS.m_formats.Any(fmt => fmt.Value == m_data_MBS.m_formatsMappings[f])),
				formats.Count);

			var colors = m_CodipressEntriesBySupports.SelectMany(s => s.Value.Select(entry => entry.couleurField)).Distinct().ToList();
			text_colors.Text = string.Format("{0} / {1}",
				colors.Count - colors.Count(c => m_data_MBS.m_colorMappings.ContainsKey(c) && m_data_MBS.m_colors.Any(col => col.Value == m_data_MBS.m_colorMappings[c])),
				colors.Count);

		}

		private void Cancel_Click(object sender, RoutedEventArgs e)
		{
			cancellationTokenSource.Cancel();
		}

		public static bool CSVFileToDataTable(string file, out DataTable datatable)
		{
			try
			{
				string[] Lines = File.ReadAllLines(file, Encoding.GetEncoding(1252)/*ANSI*/);
				string[] Fields;
				Fields = Lines[0].Split(new char[] { ';' });
				int Cols = Fields.GetLength(0);
				datatable = new DataTable();
				//1st row must be column names; force lower case to ensure matching later on.
				for (int i = 0; i < Cols; i++)
					datatable.Columns.Add(Fields[i].ToLower(), typeof(string));
				DataRow Row;
				for (int i = 1; i < Lines.GetLength(0); i++)
				{
					if (Lines[i] == "")
						continue;
					Fields = Lines[i].Split(new char[] { ';' });
					Row = datatable.NewRow();
					for (int f = 0; f < Cols; f++)
						Row[f] = Fields[f];
					datatable.Rows.Add(Row);
				}
				return true;
			}
			catch
			{
				MessageBox.Show("erreur : le fichier " + file + " est verrouillé");
				datatable = null;
				return false;
			}
		}

		private void ButtonMapSupports_Click(object sender, RoutedEventArgs e)
		{
			var window_map = new Window_map(Window_map.EMap_type.MAP_SUPPORTS, m_CodipressEntriesBySupports.Keys.ToList(), m_data_MBS);

			var result = window_map.ShowDialog();
			UpdateCounters();
		}

		private void ButtonMapFormats_Click(object sender, RoutedEventArgs e)
		{
			var formats = m_CodipressEntriesBySupports.SelectMany(s => s.Value.Select(entry => entry.UniqueFormat())).Distinct().ToList();
			var window_map = new Window_map(Window_map.EMap_type.MAP_FORMATS, formats, m_data_MBS, m_CodipressEntriesBySupports);

			var result = window_map.ShowDialog();
			UpdateCounters();
		}

		private void ButtonMapCouleurs_Click(object sender, RoutedEventArgs e)
		{
			var colors = m_CodipressEntriesBySupports.SelectMany(s => s.Value.Select(entry => entry.couleurField)).Distinct().ToList();
			var window_map = new Window_map(Window_map.EMap_type.MAP_COLORS, colors, m_data_MBS);

			var result = window_map.ShowDialog();
			UpdateCounters();
		}

		private void Affiner_Click(object sender, RoutedEventArgs e)
		{
			var supportsToUpdate = m_CodipressEntriesBySupports.Where(s => m_data_MBS.FindSupport(s.Key, false, false) != null).ToList();
			var window_map = new Window_map(Window_map.EMap_type.MAP_PARUTIONS, supportsToUpdate.Select(s => s.Key).ToList(), m_data_MBS);
			var result = window_map.ShowDialog();
			if (result == null || !result.Value)
				return;

			foreach (var support in m_CodipressEntriesBySupports)
			{
				var dateVersions = support.Value.Select(s => s.dateAppliPeriodeField).Distinct().OrderBy(k => k).ToList();

				DataBaseMBS.Support supportFound = m_data_MBS.FindSupport(support.Key, false, false);
				if (supportFound == null)
					continue;

				for (int i = 0; i < dateVersions.Count; ++i)
				{
					var tarifs = support.Value.Where(t => t.dateFinPeriodeField >= dateVersions[i] && t.dateAppliPeriodeField <= dateVersions[i]).ToList();
					tarifs.ForEach(t =>
					{
						if (m_data_MBS.m_IgnoreRubriques && t.HasRubrique() ||
							m_data_MBS.m_IgnoreDiffusionNotNATIONAL && t.HasDiffusionNotNational() ||
							m_data_MBS.m_IgnoreTypePubliciteNotCOMMERCIALE && t.HasTypePubliciteNotCommerciale())
							return;

						bool ignored;
						int colorCode = m_data_MBS.FindColorCode(t.couleurField, false, true);
						string format = m_data_MBS.FindFormat(t.UniqueFormat(), false, false, out ignored);
						if (format == null || ignored || colorCode == -1)
							return;

						uint formatIdentifier = m_data_MBS.m_formats.First(f => f.Value == format).Key;
						supportFound.AddTarif(dateVersions[i], formatIdentifier, (uint)colorCode, double.Parse(t.tarifField));
					});
				}
			}

			m_data_MBS.updateParutions();
			m_data_MBS.write(@".\");
			MessageBox.Show("les fichiers de la base MBS sont mis à jour !");
		}
	}
}

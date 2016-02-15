using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
// using System.Data.OracleClient;
using TarifsPresse.Destination.Classes;
using System.IO;
using TarifsPresse.Sources.Classes;
using System.Globalization;
using System.Xml.Serialization;
using IMP_CODI_PRESS_TARIFS_LOG;
using TarifsPresse.Destinations.Classes;
using TarifsPresse.DataSet1TableAdapters;

namespace TarifsPresse
{
    public partial class Form1 : Form
    {
        public DataSupports m_DataSupports;
        public DataTarifs m_DataTarifs;
        public DataVersions m_DataVersions;
        public DataParutions m_DataParutions;

        public DataCodipressLog m_DataCodipressLog;
        public Data m_Data;

        public Form1()
        {
            InitializeComponent();

            m_DataSupports = new DataSupports(DateTime.Today);
            m_DataTarifs = new DataTarifs(DateTime.Today);
            m_DataVersions = new DataVersions(DateTime.Today);
            m_DataParutions = new DataParutions(DateTime.Today);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            checkBoxIgnoreRubriques.Checked = m_Data.m_IgnoreRubriques;
            checkBoxIgnoreDiffusionNotNATIONAL.Checked = m_Data.m_IgnoreDiffusionNotNATIONAL;
            checkBoxIgnoreTypePubliciteNotCOMMERCIALE.Checked = m_Data.m_IgnoreTypePubliciteNotCOMMERCIALE;

            pathSupports.BackColor = Color.Red;
            pathTarifs.BackColor = Color.Red;
            pathVersions.BackColor = Color.Red;
            pathParutions.BackColor = Color.Red;

            pathCodipress.BackColor = Color.Red;
            UpdateEtape2();

            return;

/*
            string oradb = "PITAPRESSFR01.PIGE ="
                + "(DESCRIPTION ="
                + "    (ADDRESS_LIST ="
                + "        (ADDRESS = (COMMUNITY = tcp.pige) (PROTOCOL = TCP) (HOST = SUMATRA) (PORT = 1521))"
                + "    )"
                + "    (CONNECT_DATA = (SID = PITAFR01))"
                + ")";

            //MessageBox.Show("test");
            
            OracleConnection conn = new OracleConnection(oradb);
            conn.Open();

            string sql = "select * from ";
            OracleCommand cmd = new OracleCommand(sql, conn);
            cmd.CommandType = CommandType.Text;

            OracleDataReader dr = cmd.ExecuteReader();

            dr.Read();

            label1.Text = dr["department_name"].ToString(); // C# retrieve by column name
            label1.Text = dr.GetString(0).ToString();  // return a .NET data type
            label1.Text = dr.GetOracleString(0).ToString();  // return an Oracle data type

            conn.Close();
*/
        }

        private void UpdateEtape2()
        {
            DateTime _date;
            if (pathSupports.BackColor == Color.Green &&
                 pathVersions.BackColor == Color.Green &&
                 pathTarifs.BackColor == Color.Green &&
                 pathParutions.BackColor == Color.Green &&
                 DateTime.TryParseExact((new FileInfo(pathSupports.Text)).Name.Substring(2, 6),
                     "ddMMyy", CultureInfo.InvariantCulture, DateTimeStyles.None, out _date))
            {
                groupBoxEtape2.Enabled = true;
                labelEtape1.Text = "-> Les 4 fichiers sont chargés et vérifiés";

                if (m_DataCodipressLog != null && m_DataCodipressLog.Files.Count > 0)
                {
                    var d = m_DataCodipressLog.Files.Max(kvp => kvp.Key).Date;
                    if (d > _date)
                        _date = d;
                }

                if (m_Data == null)
                {
                    m_Data = new Data(_date, m_DataSupports, m_DataVersions, m_DataTarifs, m_DataParutions);
                    m_DataSupports = null;
                    m_DataVersions = null;
                    m_DataTarifs = null;
                    m_DataParutions = null;
                }

				m_Data.updateParutions();

                //m_Data.m_Date = _date;
                m_DataCodipressLog = new DataCodipressLog(m_Data);
            }
            else
            {
                groupBoxEtape2.Enabled = false;
                labelEtape1.Text = "-> Veuillez charger tous les fichiers";
            }
            Application.DoEvents();
            UpdateEtape3(0, 0, 0);
        }

        private void UpdateEtape3(uint unmappedSupports, uint unmappedColorCodes, uint unmappedFormats)
        {
            labelSupportsNotFound.Text = string.Format("{0} supports inconnus", unmappedSupports);
            labelColorCodeNotFound.Text = string.Format("{0} codes couleur inconnus", unmappedColorCodes);
            labelFormatNotFound.Text = string.Format("{0} formats inconnus", unmappedFormats);

            if (pathCodipress.BackColor == Color.Green)
            {
                groupBoxEtape3.Enabled = true;
                labelStatusImportCodipress.Text = string.Format("Il y a {0} mises à jour disponible", m_DataCodipressLog.Count);

                buttonMapSupports.Enabled = unmappedSupports > 0;
                buttonMapColorCode.Enabled = unmappedColorCodes > 0;
                buttonMapFormat.Enabled = unmappedFormats > 0;

            }
            else
            {
                groupBoxEtape3.Enabled = false;
                labelStatusImportCodipress.Text = "-> le dossier source ne contient aucune mise à jour valide";
                buttonMapSupports.Enabled = false;
                buttonMapColorCode.Enabled = false;
                buttonMapFormat.Enabled = false;
            }
            UpdateEtape4();
        }

        private void UpdateEtape4()
        {
            if (groupBoxEtape3.Enabled &&
                buttonMapSupports.Enabled == false &&
                buttonMapColorCode.Enabled == false &&
                buttonMapFormat.Enabled == false &&
                m_DataCodipressLog.Files.Count > 0)
            {
                buttonMajBases.Enabled = true;
            }
            else
            {
                //buttonMajBases.Enabled = false;
                buttonMajBases.Enabled = true;
            }
        }

        private bool OpenAndImportSourceFile(TextBox textbox, IData dataObject, string title, string extension, string filter, string errorMsg, string filename = null)
        {
            var dialog = new OpenFileDialog();
            dialog.Multiselect = false;
            dialog.CheckFileExists = true;
            dialog.CheckPathExists = true;
            dialog.DefaultExt = extension;
            dialog.Filter = filter;
            dialog.Title = title;
            dialog.InitialDirectory = Directory.GetCurrentDirectory();

            bool result = false;
            dialog.FileName = filename;

            if (filename != null || dialog.ShowDialog(this) == DialogResult.OK)
            {
                Application.DoEvents();
                this.Cursor = Cursors.WaitCursor;
                progressBarSource.Top = textbox.Top + groupBoxEtape1.Top;
                progressBarSource.Left = textbox.Left + groupBoxEtape1.Left;
                progressBarSource.Show();

                if (dataObject.read(new FileInfo(dialog.FileName), progressBarSource, DateTime.Today))
                {
                    textbox.Text = dialog.FileName;
                    textbox.BackColor = Color.Green;
                    textbox.Select(textbox.Text.Length, 0);
                    result = true;
                }
                else
                {
                    MessageBox.Show(errorMsg);
                    textbox.Text = "";
                    textbox.BackColor = Color.Red;
                    result = false;
                }
                progressBarSource.Hide();
                this.Cursor = Cursors.Default;
                Application.DoEvents();
                UpdateEtape2();
            }
            return result;
        }

        private void buttonPathSupports_Click(object sender, EventArgs e)
        {
            if (OpenAndImportSourceFile(pathSupports, m_DataSupports,
                "Choisir le fichier des supports (PSxxxxxx.lib)",
                ".lib",
                "Supports Files (PS*.lib)|PS*.lib",
                "Le fichier de supports est invalide."))
            {
                Application.DoEvents();
                var file = new FileInfo(pathSupports.Text);
                var path = file.DirectoryName;

                if (pathVersions.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathVersions, m_DataVersions, "", "", "", "Le fichier de versions de tarif est invalide.", path + '\\' + file.Name.Replace("PS", "PV").Replace(".lib", ".dat"));
                if (pathTarifs.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathTarifs, m_DataTarifs, "", "", "", "Le fichier de tarifs est invalide.", path + '\\' + file.Name.Replace("PS", "PT"));
                if (pathParutions.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathParutions, m_DataParutions, "", "", "", "Le fichier des parutions est invalide.", path + '\\' + file.Name.Replace("PS", "PP").Replace(".lib", ".dat"));
            }

        }

        private void buttonPathVersions_Click(object sender, EventArgs e)
        {
            if (OpenAndImportSourceFile(pathVersions, m_DataVersions,
                "Choisir le fichier des versions de tarif (PVxxxxxx.dat)",
                ".dat",
                "Version Files (PV*.dat)|PV*.dat",
                "Le fichier de versions de tarif est invalide."))
            {
                Application.DoEvents();
                var file = new FileInfo(pathVersions.Text);
                var path = file.DirectoryName;

                if (pathSupports.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathSupports, m_DataSupports, "", "", "", "Le fichier de supports est invalide.", path + '\\' + file.Name.Replace("PV", "PS").Replace(".dat", ".lib"));
                if (pathTarifs.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathTarifs, m_DataTarifs, "", "", "", "Le fichier de tarifs est invalide.", path + '\\' + file.Name.Replace("PV", "PT").Replace(".dat", ".lib"));
                if (pathParutions.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathParutions, m_DataParutions, "", "", "", "Le fichier des parutions est invalide.", path + '\\' + file.Name.Replace("PV", "PP"));
            }
        }

        private void buttonPathTarifs_Click(object sender, EventArgs e)
        {
            if (OpenAndImportSourceFile(pathTarifs, m_DataTarifs,
                "Choisir le fichier des tarifs (PTxxxxxx.lib)",
                ".lib",
                "Tarifs Files (PT*.lib)|PT*.lib",
                "Le fichier de tarifs est invalide."))
            {
                Application.DoEvents();
                var file = new FileInfo(pathTarifs.Text);
                var path = file.DirectoryName;

                if (pathSupports.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathSupports, m_DataSupports, "", "", "", "Le fichier de supports est invalide.", path + '\\' + file.Name.Replace("PT", "PS"));
                if (pathVersions.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathVersions, m_DataVersions, "", "", "", "Le fichier de versions de tarif est invalide.", path + '\\' + file.Name.Replace("PT", "PV").Replace(".lib", ".dat"));
                if (pathParutions.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathParutions, m_DataParutions, "", "", "", "Le fichier des parutions est invalide.", path + '\\' + file.Name.Replace("PT", "PP").Replace(".lib", ".dat"));
            }
        }

        private void buttonPathParutions_Click(object sender, EventArgs e)
        {
            if (OpenAndImportSourceFile(pathParutions, m_DataParutions,
                "Choisir le fichier des parutions (PPxxxxxx.dat)",
                ".dat",
                "Version Files (PP*.dat)|PP*.dat",
                "Le fichier des parutions est invalide."))
            {
                Application.DoEvents();
                var file = new FileInfo(pathParutions.Text);
                var path = file.DirectoryName;

                if (pathSupports.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathSupports, m_DataSupports, "", "", "", "Le fichier de supports est invalide.", path + '\\' + file.Name.Replace("PP", "PS").Replace(".dat", ".lib"));
                if (pathVersions.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathVersions, m_DataVersions, "", "", "", "Le fichier de versions de tarif est invalide.", path + '\\' + file.Name.Replace("PP", "PV"));
                if (pathTarifs.BackColor != Color.Green)
                    OpenAndImportSourceFile(pathTarifs, m_DataTarifs, "", "", "", "Le fichier de tarifs est invalide.", path + '\\' + file.Name.Replace("PP", "PT").Replace(".dat", ".lib"));
            }
        }

        private void buttonPathCodipress_Click(object sender, EventArgs e)
        {
            if (m_DataCodipressLog == null)
                return;

            var dialog = new FolderBrowserDialog();
            dialog.Description = "Choisir le chemin des fichiers log Codipress à importer";
            dialog.ShowNewFolderButton = false;
            dialog.SelectedPath = pathCodipress.Text;

            if (e == null || dialog.ShowDialog() == DialogResult.OK)
            {
                this.Cursor = Cursors.WaitCursor;
                progressBarSource.Top = pathCodipress.Top + groupBoxEtape2.Top;
                progressBarSource.Left = pathCodipress.Left + groupBoxEtape2.Left;
                progressBarSource.Show();

                if (e != null)
                    UpdateEtape2();
                uint unmappedSupports, unmappedColorCodes, unmappedFormats;
                if (m_DataCodipressLog.Scan(new DirectoryInfo(dialog.SelectedPath), progressBarSource, out unmappedSupports, out unmappedColorCodes, out unmappedFormats))
                {
                    pathCodipress.Text = dialog.SelectedPath;
                    pathCodipress.BackColor = Color.Green;
                    pathCodipress.Select(pathCodipress.Text.Length, 0);
                }
                else
                {
                    MessageBox.Show("Le dossier spécifié ne contient pas de fichier Codipress.");
                    pathCodipress.Text = "";
                    pathCodipress.BackColor = Color.Red;
                }
                progressBarSource.Hide();
                this.Cursor = Cursors.Default;

                UpdateEtape3(unmappedSupports, unmappedColorCodes, unmappedFormats);
            }
        }

        private void buttonMapSupports_Click(object sender, EventArgs e)
        {
            XmlSerializer s = new XmlSerializer(typeof(NewDataSet));
            NewDataSet log;

            foreach (var file in m_DataCodipressLog.Files)
            {
                using (var stream = file.Value.OpenRead())
                    log = (NewDataSet)s.Deserialize(stream);

                foreach (var item in log.Items)
                {
                    if (item is NewDataSetTARIF_DELETE)
                    {
                        var delete = item as NewDataSetTARIF_DELETE;
                        //TODO
                    }
                    else if (item is NewDataSetTARIF_INSERT)
                    {
                        var insert = item as NewDataSetTARIF_INSERT;
                        var support = m_Data.FindSupport(insert.UniqueSupport(), false, true);
                        if (support == null)
                        {
                            buttonPathCodipress_Click(this, null);
                            return;
                        }
                            
                    }
                }
            }
            buttonPathCodipress_Click(this, null);
        }


        private void buttonMapColorCode_Click(object sender, EventArgs e)
        {
            XmlSerializer s = new XmlSerializer(typeof(NewDataSet));
            NewDataSet log;

            foreach (var file in m_DataCodipressLog.Files)
            {
                using (var stream = file.Value.OpenRead())
                    log = (NewDataSet)s.Deserialize(stream);

                foreach (var item in log.Items)
                {
                    if (item is NewDataSetTARIF_DELETE)
                    {
                        var delete = item as NewDataSetTARIF_DELETE;
                        //TODO
                    }
                    else if (item is NewDataSetTARIF_INSERT)
                    {
                        var insert = item as NewDataSetTARIF_INSERT;
                        var colorCode = m_Data.FindColorCode(insert.Couleur, false, true);
                        if (colorCode == -1)
                        {
                            buttonPathCodipress_Click(this, null);
                            return;
                        }

                    }
                }
            }
            buttonPathCodipress_Click(this, null); ;
        }

        private void buttonMapFormat_Click(object sender, EventArgs e)
        {
            XmlSerializer s = new XmlSerializer(typeof(NewDataSet));
            NewDataSet log;

            foreach (var file in m_DataCodipressLog.Files)
            {
                using (var stream = file.Value.OpenRead())
                    log = (NewDataSet)s.Deserialize(stream);

                foreach (var item in log.Items)
                {
                    if (item is NewDataSetTARIF_DELETE)
                    {
                        var delete = item as NewDataSetTARIF_DELETE;
                        //TODO
                    }
                    else if (item is NewDataSetTARIF_INSERT)
                    {
                        var insert = item as NewDataSetTARIF_INSERT;
                        bool ignored = m_Data.IsFormatIgnored(insert);
                        if (!ignored && m_Data.FindFormat(insert.UniqueFormat(), false, true, out ignored) == null && !ignored)
                        {
                            buttonPathCodipress_Click(this, null);
                            return;
                        }
                    }
                }
            }
            buttonPathCodipress_Click(this, null); ;
        }

        private void buttonMajBases_Click(object sender, EventArgs e)
        {
            XmlSerializer s = new XmlSerializer(typeof(NewDataSet));
            NewDataSet log;
            
            foreach (var file in m_DataCodipressLog.Files)
            {
                using (var stream = file.Value.OpenRead())
                    log = (NewDataSet)s.Deserialize(stream);

                foreach (var item in log.Items)
                {
                    if (item is NewDataSetTARIF_DELETE)
                    {
                        var delete = item as NewDataSetTARIF_DELETE;
                        //TODO ?
                    }
                    else if (item is NewDataSetTARIF_INSERT)
                    {
                        var insert = item as NewDataSetTARIF_INSERT;

                        bool ignored = m_Data.IsFormatIgnored(insert);
                        if (ignored || (m_Data.FindFormat(insert.UniqueFormat(), false, false, out ignored) == null && ignored))
                            continue;
                        DateTime date;
                        //insert.DateAppliPeriode = "02/01/2013 00:00:00"
                        if (!DateTime.TryParseExact(insert.DateAppliPeriode, "dd/MM/yyyy hh:mm:ss", CultureInfo.InvariantCulture, DateTimeStyles.None, out date))
                        {
                            MessageBox.Show("La date d'application de l'insertion est dans un format non reconu,\r\nil devrait être au format \"dd/MM/yy hh:mm:ss\",\r\n" + insert.DateAppliPeriode + "\r\nInsertion ignorée");
                            continue;
                        }

                        var support = m_Data.FindSupport(insert.UniqueSupport(), false, true);

                        int colorCode = m_Data.FindColorCode(insert.Couleur, false, true);
                        string format = m_Data.FindFormat(insert.UniqueFormat(), false, true, out ignored);
                        if (ignored)
                            continue;
                        if (support == null || format == null || colorCode == -1)
                        {
                            MessageBox.Show(String.Format("La couleur {0} ou le format {1} ou le support {2} ne sont pas associés !", insert.Couleur, insert.UniqueFormat() , insert.UniqueSupport()));
                            buttonPathCodipress_Click(this, null);
                            return;
                        }
                        else
                        {
                            uint formatIdentifier = m_Data.m_formats.First(f => f.Value == format).Key;
                            m_Data.m_Data[support.m_Identifier].AddTarif(date, formatIdentifier, (uint)colorCode, uint.Parse(insert.Tarif));
                        }
                    }
                    else
                    {
                        MessageBox.Show(string.Format("Item invalide dans les mises à jour du {0}", file.Key));
                        break;
                    }
                }
                m_Data.m_Date = file.Key;
                m_Data.write(@".\");
                if (buttonMapSupports.Enabled  ||
                    buttonMapColorCode.Enabled  ||
                    buttonMapFormat.Enabled)
                    break;
            }
            buttonPathCodipress_Click(this, null);
        }

        private void checkBoxIgnoreRubriques_CheckedChanged(object sender, EventArgs e)
        {
            m_Data.m_IgnoreRubriques = checkBoxIgnoreRubriques.Checked;
            buttonPathCodipress_Click(this, null);
        }

        private void checkBoxIgnoreTypePubliciteNotCOMMERCIALE_CheckedChanged(object sender, EventArgs e)
        {
            m_Data.m_IgnoreTypePubliciteNotCOMMERCIALE = checkBoxIgnoreTypePubliciteNotCOMMERCIALE.Checked;
            buttonPathCodipress_Click(this, null);
        }

        private void checkBoxIgnoreDiffusionNotNATIONAL_CheckedChanged(object sender, EventArgs e)
        {
            m_Data.m_IgnoreDiffusionNotNATIONAL = checkBoxIgnoreDiffusionNotNATIONAL.Checked;
            buttonPathCodipress_Click(this, null);
        }

		private void buttonFileCodipress_Click(object sender, EventArgs e)
		{

		}

		private void buttonImporterBaseCodipress_Click(object sender, EventArgs e)
		{
			DataSupports otherSupports = new DataSupports(DateTime.Today);
			otherSupports.read(new FileInfo(@"C:\Work\JFC - DPS2\TarifsPresse.Head\TarifsPresse\bin\Debug\PS301213.lib"), null, DateTime.Today);
			var otherSupportsData = otherSupports.Supports.Select(support => new { identifier = support.m_Identifier, libelle = support.m_Libelle }).Distinct().ToList();

			DataTarifs otherTarifs = new DataTarifs(DateTime.Today);
			otherTarifs.read(new FileInfo(@"C:\Work\JFC - DPS2\TarifsPresse.Head\TarifsPresse\bin\Debug\PT301213.lib"), null, DateTime.Today);
			var otherTarifsData = otherTarifs.Formats;

			var dialog = new OpenFileDialog();
			dialog.Multiselect = false;
			dialog.CheckFileExists = true;
			dialog.CheckPathExists = true;
			dialog.DefaultExt = "";
			dialog.Filter = "Codipress files (*.csv)|*.csv";
			dialog.Title = "Choisir le fichier csv Codipress à importer";
			dialog.InitialDirectory = Directory.GetCurrentDirectory();

			if (dialog.ShowDialog(this) == DialogResult.OK)
			{
				string file = dialog.FileName;
				DataTable table = CSVFileToDataTable(file);

				var entries = table.Select().ToList().Select(dr => new CodipressEntry(dr)).ToList();

				var bySupports = entries.GroupBy(k => k.UniqueSupport());
				foreach (var support in bySupports)
				{
					var dateVersions = support.Select(s => s.dateAppliPeriodeField).Distinct().OrderBy(k => k).ToList();

					Data.Support supportFound = m_Data.FindSupport(support.Key, false, false);
					if (supportFound == null)
					{
						var sname = support.Key;
						if (m_Data.m_SupportMappings.ContainsKey(sname))
							sname = m_Data.m_SupportMappings[sname];
						var _s = otherSupportsData.FirstOrDefault(s => s.libelle == sname);
						if (_s != null)
						{
							m_Data.AddSupport(_s.identifier, _s.libelle);
							supportFound = m_Data.FindSupport(sname, false, false);
						}
						else
						{
							while (supportFound == null)
								supportFound = m_Data.FindSupport(support.Key, false, true);
						}
					}


					for (int i = 0; i < dateVersions.Count; ++i)
					{
						var tarifs = support.Where(t => t.dateFinPeriodeField >= dateVersions[i] && t.dateAppliPeriodeField <= dateVersions[i]).ToList();
						tarifs.ForEach(t => {
							if (m_Data.m_IgnoreRubriques && t.HasRubrique() ||
								m_Data.m_IgnoreDiffusionNotNATIONAL && t.HasDiffusionNotNational() ||
								m_Data.m_IgnoreTypePubliciteNotCOMMERCIALE && t.HasTypePubliciteNotCommerciale())
								return;

							int colorCode = m_Data.FindColorCode(t.couleurField, false, true);
							bool ignored;
							string format = m_Data.FindFormat(t.UniqueFormat(), false, false, out ignored);
							if (ignored)
								return;
							if (format == null)
							{
								var fname = t.UniqueFormat();
								if (m_Data.m_formatsMappings.ContainsKey(fname))
									fname = m_Data.m_formatsMappings[fname];
								if (otherTarifsData.ContainsValue(fname))
								{
									var _f = otherTarifsData.First(f => f.Value == fname);
									m_Data.AddFormat(_f.Key, _f.Value);
									format = m_Data.FindFormat(t.UniqueFormat(), false, false, out ignored);
								}
								else
								{
									format = m_Data.FindFormat(t.UniqueFormat(), false, true, out ignored);
									if (ignored)
										return;
								}
							}
							if (support != null && format != null && colorCode != -1)
							{
								uint formatIdentifier = m_Data.m_formats.First(f => f.Value == format).Key;
								supportFound.AddTarif(dateVersions[i], formatIdentifier, (uint)colorCode, double.Parse(t.tarifField));
							}
						});						
					}
				}
			}
		}

		public static DataTable CSVFileToDataTable(string file)
		{
			string[] Lines = File.ReadAllLines(file, Encoding.GetEncoding(1252)/*ANSI*/);
			string[] Fields;
			Fields = Lines[0].Split(new char[] { ';' });
			int Cols = Fields.GetLength(0);
			DataTable dt = new DataTable();
			//1st row must be column names; force lower case to ensure matching later on.
			for (int i = 0; i < Cols; i++)
				dt.Columns.Add(Fields[i].ToLower(), typeof(string));
			DataRow Row;
			for (int i = 1; i < Lines.GetLength(0); i++)
			{
				if (Lines[i] == "")
					continue;
				Fields = Lines[i].Split(new char[] { ';' });
				Row = dt.NewRow();
				for (int f = 0; f < Cols; f++)
					Row[f] = Fields[f];
				dt.Rows.Add(Row);
			}
			return dt;
		}
    }
}

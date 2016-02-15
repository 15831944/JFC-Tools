using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Globalization;
using System.Xml.Serialization;
using IMP_CODI_PRESS_TARIFS_LOG;
using TarifsPresse.Destination.Classes;
using TarifsPresse.Destinations.Classes;


namespace TarifsPresse.Sources.Classes
{
    public class DataCodipressLog
    {
        Dictionary<DateTime, FileInfo> m_Files;
        Data m_Data;

        public DataCodipressLog(Data data)
        {
            m_Files = new Dictionary<DateTime, FileInfo>();
            m_Data = data;
        }

        public int Count
        {
            get { return m_Files.Count; }
        }

        public Dictionary<DateTime, FileInfo> Files
        {
            get { return m_Files; }
        }

        public bool Scan(DirectoryInfo dir, ProgressBar progressCtrl, out uint unmappedSupportsCount, out uint unmappedColorCodesCount, out uint unmappedFormatsCount)
        {
            unmappedSupportsCount = 0;
            unmappedColorCodesCount = 0;
            unmappedFormatsCount = 0;
            m_Files.Clear();

            var files = dir.EnumerateFiles("IMP_CODI_PRESS_TARIFS*log.xml").ToList();
            if (files.Count == 0)
                return false;

            progressCtrl.Minimum = 0;
            progressCtrl.Maximum = files.Count;
            progressCtrl.Step = 1;

            var supports = new List<string>();
            var formats = new List<string>();
            var colors = new List<string>();

            XmlSerializer s = new XmlSerializer(typeof(NewDataSet));
            NewDataSet log;

            foreach (FileInfo file in files)
            {
                using (var stream = file.OpenRead())
                {
                    try
                    {
                        log = (NewDataSet)s.Deserialize(stream);
                    }
                    catch
                    {
                        MessageBox.Show(string.Format("Le format du fichier de log {0} est inattendu !", file.Name));
                        continue;
                    }
                }

                if (log.Items == null || log.Items.Length == 0)
                    continue;// il n'y a pas de modification pour ce jour

                string date = file.Name.Substring("IMP_CODI_PRESS_TARIFS".Length);
                date = date.Substring(0, date.Length - "log.xml".Length);
                DateTime _date;
                if (DateTime.TryParseExact(date, "yyyyMMddhhmmss",
                    CultureInfo.InvariantCulture, DateTimeStyles.None,
                    out _date) && _date.Date > m_Data.m_Date)
                    m_Files.Add(_date, file);
                else
                    continue;

                // count unmapped supports, formats and color codes
                foreach (var item in log.Items)
                {
                    if (item is NewDataSetTARIF_DELETE)
                        continue;
                    NewDataSetTARIF_INSERT insert = (item as NewDataSetTARIF_INSERT);
                    var support = insert.UniqueSupport();
                    if (m_Data.FindSupport(support, false) == null)
                        supports.Add(support);

                    bool ignored = m_Data.IsFormatIgnored(insert);
                    if (!ignored && m_Data.FindFormat(insert.UniqueFormat(), false, false, out ignored) == null && !ignored)
                        formats.Add(insert.UniqueFormat());

                    if (m_Data.FindColorCode(insert.Couleur, false) == -1)
                        colors.Add(insert.Couleur);

                }
                progressCtrl.Increment(1);
            }
            unmappedSupportsCount = (uint)supports.Distinct().Count();
            unmappedFormatsCount = (uint)formats.Distinct().Count();
            unmappedColorCodesCount = (uint)colors.Distinct().Count();
            return true;
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using TarifsPresse.Destination.Classes;
using TarifsPresse.Destinations.Classes;

namespace TarifsPresse
{
    public partial class FormFormats : Form
    {
        Data m_Data;
        string m_FormatToMap;

        public string m_MappedFormat { get; set; }

        public FormFormats(Data data, string formatToMap)
        {
            m_Data = data;
            m_FormatToMap = formatToMap;

            InitializeComponent();
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewFormat.SelectedItems.Count > 0)
            {
                textBoxSelectedFormat.Text = listViewFormat.SelectedItems[0].Text;
                textBoxSelectedFormatCode.Text = m_Data.m_formats.First(f => f.Value == listViewFormat.SelectedItems[0].Text).Key.ToString();
                buttonMapperFormat.Enabled = true;
            }
        }

        private void FormFormats_Load(object sender, EventArgs e)
        {
            textBoxFormatToMap.Text = m_FormatToMap;
            textBoxFormatToCreate.Text = m_FormatToMap;

            var supports = m_Data.m_formats.OrderBy(f => f.Value).Select(f => f.Value).Select(f => new ListViewItem(f)).ToArray();
            listViewFormat.Items.AddRange(supports);
			var item = listViewFormat.FindItemWithText(m_FormatToMap.Substring(0, 2));
            if (item != null)
                listViewFormat.EnsureVisible(item.Index);
        }

        private void FormFormats_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_MappedFormat != null)
                return;
            var result = MessageBox.Show("Voulez-vous vraiment abandonner cette opération ?", "Annuler l'opération", MessageBoxButtons.YesNo);
            if (result == DialogResult.Yes)
            {
                //rien a faire ici
            }
            else if (result == DialogResult.No)
            {
                e.Cancel = true;
            }
            else
            {
                throw new Exception();
            }
        }

        private void buttonMapperFormat_Click(object sender, EventArgs e)
        {
            m_MappedFormat = textBoxSelectedFormat.Text;
            m_Data.AddFormatMapping(m_FormatToMap, textBoxSelectedFormat.Text);
            DialogResult = DialogResult.OK;
            Close();
        }

        private void buttonCreerFormat_Click(object sender, EventArgs e)
        {
            if (m_Data.AddFormat(uint.Parse(textBoxFormatCodeToCreate.Text), textBoxFormatToCreate.Text))
            {
                m_MappedFormat = textBoxFormatToCreate.Text;
                if (!m_FormatToMap.Equals(m_MappedFormat))
                    m_Data.AddFormatMapping(m_FormatToMap, m_MappedFormat);
                DialogResult = DialogResult.OK;
                Close();
            }
            else
            {
                MessageBox.Show("Le format ne peut être créé.");
            }
        }

        private void textBoxFormatCodeToCreate_TextChanged(object sender, EventArgs e)
        {
            uint code;
            if (textBoxFormatCodeToCreate.Text.Length == 0 ||
                !uint.TryParse(textBoxFormatCodeToCreate.Text, out code) ||
                m_Data.FormatIdentifierExists(code))
            {
                textBoxFormatCodeToCreate.BackColor = Color.Red;
                buttonCreerFormat.Enabled = false;
            }
            else
            {
                textBoxFormatCodeToCreate.BackColor = Color.White;
                bool ignored;
                if (textBoxFormatToCreate.Text.Length != 0 &&
                    m_Data.FindFormat(textBoxFormatToCreate.Text, false, false, out ignored) == null)
                    buttonCreerFormat.Enabled = true;
                else
                    buttonCreerFormat.Enabled = false;
            }
        }

        private void textBoxFormatToCreate_TextChanged(object sender, EventArgs e)
        {
            bool ignored;
            if (textBoxFormatToCreate.Text.Length == 0 ||
                m_Data.FindFormat(textBoxFormatToCreate.Text, false, false, out ignored) != null)
            {
                textBoxFormatToCreate.BackColor = Color.Red;
                buttonCreerFormat.Enabled = false;
            }
            else
            {
                textBoxFormatToCreate.BackColor = Color.White;
                uint code;
                if (textBoxFormatCodeToCreate.Text.Length != 0 &&
                    uint.TryParse(textBoxFormatCodeToCreate.Text, out code) &&
                    !m_Data.FormatIdentifierExists(code))
                    buttonCreerFormat.Enabled = true;
                else
                    buttonCreerFormat.Enabled = false;
            }
        }

        private void textBoxFormatCodeToCreate_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && buttonCreerFormat.Enabled == true)
                buttonCreerFormat_Click(this, null);
        }

        private void buttonIgnoreFormat_Click(object sender, EventArgs e)
        {
            m_Data.AddFormatIgnored(m_FormatToMap);
            m_MappedFormat = m_FormatToMap;
            DialogResult = DialogResult.OK;
            Close();
        }
    }
}

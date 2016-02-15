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
    public partial class FormSupports : Form
    {
        Data m_Data;
        string m_SupportToMap;

        public Data.Support m_MappedSupport;

        public FormSupports(Data data, string supportToMap)
        {
            m_Data = data;
            m_SupportToMap = supportToMap;

            InitializeComponent();
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewSupports.SelectedItems.Count > 0)
            {
                var s = m_Data.FindSupport(listViewSupports.SelectedItems[0].Text, false);

                textBoxSelectedSupport.Text = m_Data.m_Data[s.m_Identifier].m_Libelle;
                textBoxSelectedSupportCode.Text = m_Data.m_Data[s.m_Identifier].m_Identifier;
                buttonMapperSupport.Enabled = true;
            }
        }

        private void FormSupports_Load(object sender, EventArgs e)
        {
            textBoxSupportToMap.Text = m_SupportToMap;
            textBoxSupportToCreate.Text = m_SupportToMap;

            var supports = m_Data.m_Data.Select(s => s.Value.m_Libelle).Distinct().Select(s => new ListViewItem(s)).ToList();
            //supports.AddRange(m_Supports.Mappings.Select(m => new ListViewItem(m.Key)).ToList());
            listViewSupports.Items.AddRange(supports.OrderBy(s => s.Text).ToArray());
            var item = listViewSupports.FindItemWithText(m_SupportToMap.Substring(0, 1));
            if (item != null)
                listViewSupports.EnsureVisible(item.Index);
        }

        private void FormSupports_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_MappedSupport != null)
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

        private void buttonMapperSupport_Click(object sender, EventArgs e)
        {
            m_MappedSupport = m_Data.FindSupport(textBoxSelectedSupport.Text, false);
            m_Data.AddSupportMapping(m_SupportToMap, m_MappedSupport.m_Libelle);
            DialogResult = DialogResult.OK;
            Close();
        }

        private void buttonCreerSupport_Click(object sender, EventArgs e)
        {
            if (m_Data.AddSupport(textBoxSupportCodeToCreate.Text, textBoxSupportToCreate.Text))
            {
                m_MappedSupport = m_Data.FindSupport(textBoxSupportToCreate.Text, false);
                if (!m_SupportToMap.Equals(m_MappedSupport.m_Libelle))
                    m_Data.AddSupportMapping(m_SupportToMap, m_MappedSupport.m_Libelle);
                DialogResult = DialogResult.OK;
                Close();
            }
            else
            {
                MessageBox.Show("Le support ne peut être créé.");
            }
        }

        private void textBoxSupportCodeToCreate_TextChanged(object sender, EventArgs e)
        {
            if (textBoxSupportCodeToCreate.Text.Length == 0 ||
                m_Data.m_Data.ContainsKey(textBoxSupportCodeToCreate.Text))
            {
                textBoxSupportCodeToCreate.BackColor = Color.Red;
                buttonCreerSupport.Enabled = false;
            }
            else
            {
                textBoxSupportCodeToCreate.BackColor = Color.White;
                buttonCreerSupport.Enabled = true;
            }
        }

        private void textBoxSupportToCreate_TextChanged(object sender, EventArgs e)
        {
            if (textBoxSupportToCreate.Text.Length == 0 ||
                m_Data.FindSupport(textBoxSupportToCreate.Text, false) != null)
            {
                textBoxSupportToCreate.BackColor = Color.Red;
                buttonCreerSupport.Enabled = false;
            }
            else
            {
                textBoxSupportToCreate.BackColor = Color.White;
                buttonCreerSupport.Enabled = true;
            }
        }

        private void textBoxSupportCodeToCreate_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && buttonCreerSupport.Enabled == true)
                buttonCreerSupport_Click(this, null);
        }
    }
}

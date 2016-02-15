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
    public partial class FormCouleurs : Form
    {
        Data m_Data;
        string m_CouleurToMap;

        public uint m_MappedCouleur { get; set; }

        public FormCouleurs(Data data, string couleurToMap)
        {
            m_Data = data;
            m_CouleurToMap = couleurToMap;

            InitializeComponent();
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewCouleurs.SelectedItems.Count > 0)
            {
                int code = m_Data.FindColorCode(listViewCouleurs.SelectedItems[0].Text, false);
                if (code == -1)
                {
                    m_MappedCouleur = 0;
                    DialogResult = DialogResult.Cancel;
                    Close();
                }
                textBoxSelectedCouleur.Text = m_Data.m_colors[(uint)code];
                textBoxSelectedCouleurCode.Text = code.ToString();
                buttonMapperCouleur.Enabled = true;
            }
        }

        private void FormCouleurs_Load(object sender, EventArgs e)
        {
            textBoxCouleurToMap.Text = m_CouleurToMap;

            var couleurs = m_Data.m_colors.Select(c => c.Value).Select(c => new ListViewItem(c)).ToList();
            listViewCouleurs.Items.AddRange(couleurs.OrderBy(s => s.Text).ToArray());
            var item = listViewCouleurs.FindItemWithText(m_CouleurToMap.Substring(0, 1));
            if (item != null)
                listViewCouleurs.EnsureVisible(item.Index);
        }

        private void FormCouleurs_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (m_MappedCouleur == 0)
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

        private void buttonMapperCouleur_Click(object sender, EventArgs e)
        {
            m_MappedCouleur = uint.Parse(textBoxSelectedCouleurCode.Text);
            m_Data.AddColorMapping(m_CouleurToMap, textBoxSelectedCouleur.Text);
            DialogResult = DialogResult.OK;
            Close();
        }
    }
}

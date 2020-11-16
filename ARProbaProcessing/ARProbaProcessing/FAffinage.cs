using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace ARProbaProcessing
{
    public partial class FAffinage : Form
    {
        public FAffinage()
        {
            InitializeComponent();
            cbo_panel.SelectedIndex = 0;
        }

        public int GetYear()
        {
            return Convert.ToInt32(nudYear.Value);
        }

        public Enquete GetEnquete()
        {
            switch (cbo_panel.SelectedItem.ToString().ToUpper())
            {
                case "NATIONAL":
                    return Enquete.PanelNational;
                case "CADRE":
                    return Enquete.PanelCadre;
                case "ILE DE FRANCE":
                    return Enquete.PanelIleDeFrance;
                default:
                    throw new Exception($"Enquete non définie : {cbo_panel.SelectedText}");
            }
        }

        public string GetDirName()
        {
            switch (cbo_panel.SelectedItem.ToString().ToUpper())
            {
                case "NATIONAL":
                    return "Panel_National";
                case "CADRE":
                    return "Panel_Cadre";
                case "ILE DE FRANCE":
                    return "Panel_Iledefrance";
                default:
                    throw new Exception($"Enquete non définie : {cbo_panel.SelectedText}");
            }
        }
    }
}

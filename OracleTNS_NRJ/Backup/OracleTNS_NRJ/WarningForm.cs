using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AtOracle_NRJ
{
    public partial class WarningForm : Form
    {
        
        public String MyLabelChange 
        {
            get { return this.labelError.Text; }
            set { this.labelError.Text = value; }

           
        }

        public WarningForm()// string connecString string error
        {
            InitializeComponent();
            ShowDialog();

        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            //new ConnecOracle(txtConnexion.Text,txtArgs.Text);
            
            this.Close();
            //Application.Exit();
        }



    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TarifsPresse
{
	public partial class FormMatchSupport : Form
	{
		private List<string> list;
		private string toMap;

		public String selected { get; set; }

		public FormMatchSupport(String toMap, List<string> list)
		{
			// TODO: Complete member initialization
			this.list = list;
			this.toMap = toMap;
			selected = null;
			InitializeComponent();
		}

		private void FormMatchSupport_Load(object sender, EventArgs e)
		{
			label2.Text = toMap;
			listBox1.Sorted = false;
			listBox1.Items.AddRange(list.ToArray());
		}

		private void buttonAssocier_Click(object sender, EventArgs e)
		{
			
		}

		private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			selected = listBox1.SelectedItem as string;
		}
	}
}

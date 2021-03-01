using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Facility_Management
{
    public partial class GetElements : Form
    {

		public GetElements()
		{
			InitializeComponent();
		}
		
		private void ComboBoxVersion_KeyPress(object sender, KeyPressEventArgs e)
		{
			e.KeyChar = '\0';
		}

		private void ButtonGet_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void ButtonCancel_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void ButtonIgnore_Click(object sender, EventArgs e)
		{
			Close();
		}
	}
}

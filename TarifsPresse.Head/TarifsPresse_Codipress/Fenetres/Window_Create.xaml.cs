using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace TarifsPresse_Codipress.Fenetres
{
	/// <summary>
	/// Logique d'interaction pour Window_Create.xaml
	/// </summary>
	public partial class Window_Create : Window
	{
		DataBaseMBS m_Database_MBS;
		Window_map.EMap_type m_Type;
		String m_Libelle;

		#region properties
		public String Libelle
		{
			get { return (String)GetValue(LibelleProperty); }
			set { SetValue(LibelleProperty, value); }
		}

		// Using a DependencyProperty as the backing store for Libelle.  This enables animation, styling, binding, etc...
		public static readonly DependencyProperty LibelleProperty =
			DependencyProperty.Register("Libelle", typeof(String), typeof(Window_Create), new PropertyMetadata(""));


		public String Identifier
		{
			get { return (String)GetValue(IdentifierProperty); }
			set { SetValue(IdentifierProperty, value); }
		}

		// Using a DependencyProperty as the backing store for Identifier.  This enables animation, styling, binding, etc...
		public static readonly DependencyProperty IdentifierProperty =
			DependencyProperty.Register("Identifier", typeof(String), typeof(Window_Create), new PropertyMetadata(""));
		#endregion

		public Window_Create(Window_map.EMap_type type, String libelle, DataBaseMBS database_MBS, out bool created)
		{
			created = false;
			m_Database_MBS = database_MBS;
			m_Type = type;
			m_Libelle = libelle;
			Libelle = libelle.Replace(" EDITION NATIONALE", "");

			DataContext = this;
			InitializeComponent();

			switch (m_Type)
			{
				case Window_map.EMap_type.MAP_SUPPORTS:
					Title = "Création de support : " + m_Libelle;
					text_libelle.Content = "Libellé du support";
					text_identifier.Content = "Identifiant du support";
					break;
				case Window_map.EMap_type.MAP_FORMATS:
					Title = "Création de format : " + m_Libelle;
					text_libelle.Content = "Libellé du format";
					text_identifier.Content = "Identifiant du format";
					break;
// 				case Window_map.EMap_type.MAP_COLORS:
// 					Title = "Création de couleur : " + m_Libelle;
// 					text_libelle.Content = "Libellé de la couleur";
// 					text_identifier.Content = "Identifiant du couleur";
// 					break;
			}
			Button_creer.IsEnabled = false;
			
			Random rnd = new Random();
			int trycount = 0;
			bool valid = false;
			while (!valid && trycount < 100)
			{
				switch (m_Type)
				{
					case Window_map.EMap_type.MAP_SUPPORTS:
						var concat = new List<char>();
						if (trycount > 0)
							concat = concat.Concat(trycount.ToString()).ToList();
						Identifier = Libelle.Where((ch, index) => index == 0 || Libelle[index - 1] == ' ')
							.Concat(concat)
							.Take(6)
							.Aggregate("", (s, ch) => s += ch);
						valid = !m_Database_MBS.m_Data.Keys.Any(k => k.CompareTo(Identifier) == 0);
						break;
					case Window_map.EMap_type.MAP_FORMATS:
						uint random = (uint)rnd.Next(1, 2000000);
						valid = !m_Database_MBS.m_formats.ContainsKey(random);
						if (valid)
							Identifier = random.ToString();
						break;
				}
				trycount++;
			}

			if (Button_creer.IsEnabled)// on cree
			{
				Button_Click(null, null);
				created = true;
			}
		}

		private void Button_Click(object sender, RoutedEventArgs e)
		{
			switch (m_Type)
			{
				case Window_map.EMap_type.MAP_SUPPORTS:
					m_Database_MBS.AddSupport(Identifier, Libelle);
					m_Database_MBS.AddSupportMapping(m_Libelle, Libelle);
					break;
				case Window_map.EMap_type.MAP_FORMATS:
					m_Database_MBS.AddFormat(uint.Parse(Identifier), Libelle);
					m_Database_MBS.AddFormatMapping(m_Libelle, Libelle);
					break;
// 				case Window_map.EMap_type.MAP_COLORS:
// 					m_Database_MBS.AddColor(uint.Parse(Identifier), Libelle);
//					m_Database_MBS.AddFormatMapping(m_Libelle, Libelle);
// 					break;
			}

			if (this.IsLoaded)
			{
				DialogResult = true;
				Close();
			}
		}

		private void TextBox_Libelle_TextChanged(object sender, TextChangedEventArgs e)
		{
			Libelle = (sender as TextBox).Text;
			bool found = true;			
			switch (m_Type)
			{
				case Window_map.EMap_type.MAP_SUPPORTS:
					found = Libelle != null && m_Database_MBS.FindSupport(Libelle, false, false) != null;
					break;
				case Window_map.EMap_type.MAP_FORMATS:
					bool ignore;
					found = Libelle != null && m_Database_MBS.FindFormat(Libelle, false, false, out ignore) != null;
					break;
// 				case Window_map.EMap_type.MAP_COLORS:
// 					found = Libelle != null && m_Database_MBS.m_colors.ContainsValue(Libelle);
// 					break;
			}

			if (!found)
				TextBox_Libelle.Background = null;
			else
				TextBox_Libelle.Background = Brushes.Red;

			Button_creer.IsEnabled = TextBox_Libelle != null && TextBox_Identifier != null && TextBox_Libelle.Background == null && TextBox_Identifier.Background == null;
		}

		private void TextBox_Identifier_TextChanged(object sender, TextChangedEventArgs e)
		{
			Identifier = (sender as TextBox).Text;
			bool found = true;			
			uint code;
			switch (m_Type)
			{
				case Window_map.EMap_type.MAP_SUPPORTS:
						found = Identifier.Length >= 6 || Identifier != null && m_Database_MBS.m_Data.Keys.Any(k => k == Identifier);
					break;
				case Window_map.EMap_type.MAP_FORMATS:
					if (Identifier.Length <= 10 && uint.TryParse(Identifier, out code))
						found = Identifier != null && m_Database_MBS.m_formats.ContainsKey(code);
					else
						found = true;
					break;
// 				case Window_map.EMap_type.MAP_COLORS:
// 					if (Identifier.Length == 1 && uint.TryParse(Identifier, out code))
// 						found = Identifier != null && m_Database_MBS.m_colors.ContainsKey(code);
// 					else
// 						found = true;
// 					break;
			}

			if (!found)
				TextBox_Identifier.Background = null;
			else
				TextBox_Identifier.Background = Brushes.Red;
			Button_creer.IsEnabled = TextBox_Libelle != null && TextBox_Identifier != null && TextBox_Libelle.Background == null && TextBox_Identifier.Background == null;
		}
	}
}

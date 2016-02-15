using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace TarifsPresse_Codipress.Fenetres
{
	/// <summary>
	/// Logique d'interaction pour Window_map.xaml
	/// </summary>
	public partial class Window_map : Window
	{
		public enum EMap_type
		{
			MAP_SUPPORTS = 0,
			MAP_FORMATS,
			MAP_COLORS,
			MAP_PARUTIONS
		}

		public class Element : INotifyPropertyChanged
		{
			private String m_Name;
			public String Name
			{
				get { return m_Name; }
				set
				{
					if (m_Name != value)
					{
						m_Name = value;
						if (PropertyChanged != null)
							PropertyChanged(this, new PropertyChangedEventArgs("Name"));
					}
				}
			}

			private bool? m_Mapped = null;
			public bool? Mapped
			{
				get { return m_Mapped; }
				set
				{
					if (m_Mapped != value)
					{
						m_Mapped = value;
						if (PropertyChanged != null)
							PropertyChanged(this, new PropertyChangedEventArgs("Mapped"));
					}
				}
			}

			public event PropertyChangedEventHandler PropertyChanged;
		}

		public ObservableCollection<Element> ElementsToMap
		{
			get { return (ObservableCollection<Element>)GetValue(ElementsToMapProperty); }
			set { SetValue(ElementsToMapProperty, value); }
		}

		// Using a DependencyProperty as the backing store for ElementsToMap.  This enables animation, styling, binding, etc...
		public static readonly DependencyProperty ElementsToMapProperty =
			DependencyProperty.Register("ElementsToMap", typeof(ObservableCollection<Element>), typeof(Window_map), new PropertyMetadata(null));

		

		public ObservableCollection<Element> ElementsMapped
		{
			get { return (ObservableCollection<Element>)GetValue(ElementsMappedProperty); }
			set { SetValue(ElementsMappedProperty, value); }
		}

		// Using a DependencyProperty as the backing store for ElementsMapped.  This enables animation, styling, binding, etc...
		public static readonly DependencyProperty ElementsMappedProperty =
			DependencyProperty.Register("ElementsMapped", typeof(ObservableCollection<Element>), typeof(Window_map), new PropertyMetadata(null));


		public ObservableCollection<String> ElementsSupports
		{
			get { return (ObservableCollection<String>)GetValue(ElementsSupportsProperty); }
			set { SetValue(ElementsSupportsProperty, value); }
		}

		// Using a DependencyProperty as the backing store for ElementsSupports.  This enables animation, styling, binding, etc...
		public static readonly DependencyProperty ElementsSupportsProperty =
			DependencyProperty.Register("ElementsSupports", typeof(ObservableCollection<String>), typeof(Window_map), new PropertyMetadata(null));

		EMap_type m_Type;
		DataBaseMBS m_Data_MBS;
		Dictionary<String, String> m_Mappings;
		Levenshtein levenshtein = new Levenshtein();
		string[] levenshtein_Source;
		List<string> m_Ignores = new List<string>();
		Dictionary<String, List<CodipressEntry>> m_CodipressEntriesBySupports;

		public Window_map(EMap_type type, List<String> elemToMap, DataBaseMBS data_MBS, Dictionary<String, List<CodipressEntry> > codipressEntriesBySupports = null)
		{
			m_Type = type;
			m_Data_MBS = data_MBS;
			m_CodipressEntriesBySupports = codipressEntriesBySupports;

			switch (m_Type)
			{
				case EMap_type.MAP_SUPPORTS:
					levenshtein_Source = m_Data_MBS.m_Data.Select(s => s.Value.m_Libelle).ToArray();
					m_Mappings = m_Data_MBS.m_SupportMappings;
					m_Ignores = m_Data_MBS.m_supportsIgnored;
					break;
				case EMap_type.MAP_FORMATS:
					levenshtein_Source = m_Data_MBS.m_formats.Select(f => f.Value).ToArray();
					m_Mappings = m_Data_MBS.m_formatsMappings;
					m_Ignores = m_Data_MBS.m_formatsIgnored;
					break;
				case EMap_type.MAP_COLORS:
					levenshtein_Source = m_Data_MBS.m_colors.Select(f => f.Value).ToArray();
					m_Mappings = m_Data_MBS.m_colorMappings;
					break;
				case EMap_type.MAP_PARUTIONS:
					DataTable mappings_TNS;
					if (MainWindow.CSVFileToDataTable(@".\supportsTNS_Codipresse.csv", out mappings_TNS))
					{
						var columns = mappings_TNS.Columns.Cast<DataColumn>().Select(c => c.ColumnName).ToList();
						var mappings = mappings_TNS.Select().Select(r => columns.ToDictionary(col => col, col => r[col] as String)).ToList();
						
						levenshtein_Source = mappings.Select(r => r["supports codipresse"]).ToArray();
						m_Mappings = m_Data_MBS.m_ParutionsMappings;
					}
					else
						return;
					break;
			}
			levenshtein.SetupTextSentences(levenshtein_Source);

			ElementsToMap = new ObservableCollection<Element>(elemToMap
				.OrderBy(k => k)
				.Select(e => new Element
				{
					Name = e,
					Mapped = m_Ignores.Contains(e) ? null : new Nullable<bool>(m_Mappings.Keys.Any(m => m == e) && m_Mappings[e] != null && levenshtein_Source.Any(m => m == m_Mappings[e]))
				}));

			DataContext = this;

			InitializeComponent();
		}

		private void Valider_Click(object sender, RoutedEventArgs e)
		{
			this.DialogResult = true;
			Close();
		}

		private void Creer_Click(object sender, RoutedEventArgs e)
		{
			var element = ListBox_ElemsToMap.SelectedItem as Element;
			if (element == null)
				return;

			bool created;
			var wnd = new Window_Create(m_Type, element.Name, m_Data_MBS, out created);
			var result = created ? new bool?(true) : wnd.ShowDialog();// on saute l'affichage de la dialog si l'element est cree automatiquement
			if (result != null && result.Value && m_Mappings.ContainsKey(element.Name))
			{
				var new_source = levenshtein_Source.ToList();
				new_source.Add(m_Mappings[element.Name]);
				levenshtein_Source = new_source.ToArray();
				levenshtein.SetupTextSentences(levenshtein_Source);

				element.Mapped = true;
				//ListBox_ElemsToMap_SelectionChanged(ListBox_ElemsToMap, null);
				try
				{
					ListBox_ElemsToMap.SelectedItem = ElementsToMap.FirstOrDefault(el => el.Mapped == false);
					if (ListBox_ElemsToMap.SelectedItem != null)
						ListBox_ElemsToMap.ScrollIntoView(ListBox_ElemsToMap.SelectedItem);
				}
				catch
				{
					ListBox_ElemsToMap.SelectedItem = null;
				}
			}
		}

		private void ListBox_ElemsToMap_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			var lb = sender as ListBox;
			var item = lb.SelectedItem as Element;

			if (item == null)
				return;

			lb.ScrollIntoView(lb.SelectedItem);

			if (ToggleButton_AZ.IsChecked != null && ToggleButton_AZ.IsChecked.Value)
			{
				// alphabetique
				ElementsMapped = new ObservableCollection<Element>(levenshtein_Source.Select(m =>
					new Element()
					{
						Name = m,
						Mapped = m_Mappings.Keys.Contains(item.Name) ? m_Mappings[item.Name] == m : false
					}
				).OrderBy(m => m.Name));
			}
			else
			{
				var scores = levenshtein.SearchSentenceRank(item.Name);
				var matches = scores.SelectMany(s => s.Value.Select(r => levenshtein_Source[r])).ToList();

				//affinité
				ElementsMapped = new ObservableCollection<Element>(matches.Select(m =>
					new Element()
					{
						Name = m,
						Mapped = m_Mappings.Keys.Contains(item.Name) ? m_Mappings[item.Name] == m : false
					}
				));
			}

			if (m_Type == EMap_type.MAP_FORMATS)
			{
				var supports = m_CodipressEntriesBySupports
					.Where(s => 
					{
						return s.Value
							.Select(v => v.UniqueFormat())
							.Contains(item.Name);
					})
					.Select(s => s.Key).ToList();
				ElementsSupports = new ObservableCollection<string>(supports);
			}

			Ignore_button_update_flag = true;
			Ignore_button.IsChecked = item.Mapped == null;
			Ignore_button_update_flag = false;

			if (ListBox_Mapped != null)
				ListBox_Mapped.UnselectAll();
		}

		private void TextBlock_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
		{
			Ignore_button.IsChecked = false;// clear ignore

			var element = ListBox_ElemsToMap.SelectedItem as Element;
			ElementsMapped.ToList().ForEach(elem => elem.Mapped = false);
			m_Mappings[element.Name] = (sender as TextBlock).Text;
			ElementsMapped.First(el => el.Name == (sender as TextBlock).Text).Mapped = true;
			element.Mapped = true;
		}

		private void ToggleButton_AZ_State_Changed(object sender, RoutedEventArgs e)
		{
			ListBox_ElemsToMap_SelectionChanged(ListBox_ElemsToMap, null);
		}

		private void TextBlock_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
		{
			var tb = sender as TextBlock;
			ElementsMapped.ToList().ForEach(elem => elem.Mapped = false);
			m_Mappings.Remove(tb.Text);
			var element = ListBox_ElemsToMap.SelectedItem as Element;
			element.Mapped = false;
			Ignore_button.IsChecked = false;// clear ignore
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			if (m_Type == EMap_type.MAP_COLORS || m_Type == EMap_type.MAP_PARUTIONS)
				Creer.Visibility = Visibility.Hidden;
			if (m_Type != EMap_type.MAP_FORMATS && m_Type != EMap_type.MAP_SUPPORTS)
				Ignore_button.Visibility = Visibility.Hidden;

			if (m_Type != EMap_type.MAP_FORMATS)
			{
				Grid.SetRowSpan(ListBox_Mapped, 2);
				ListBox_Supports.Visibility = Visibility.Hidden;
				Label_Supports.Visibility = Visibility.Hidden;
			}

			try
			{
				ListBox_ElemsToMap.SelectedItem = ElementsToMap.FirstOrDefault(el => el.Mapped == false);
				if (ListBox_ElemsToMap.SelectedItem != null)
					ListBox_ElemsToMap.ScrollIntoView(ListBox_ElemsToMap.SelectedItem);
			}
			catch
			{
				ListBox_ElemsToMap.SelectedItem = null;
			}
		}

		private bool Ignore_button_update_flag = false;
		private void Ignore_button_State_Changed(object sender, RoutedEventArgs e)
		{
			if (!Ignore_button_update_flag && ListBox_ElemsToMap.SelectedItem != null)
			{
				var element = ListBox_ElemsToMap.SelectedItem as Element;
				ElementsMapped.ToList().ForEach(elem => elem.Mapped = false);
				m_Mappings.Remove(element.Name);

				if (Ignore_button != null && Ignore_button.IsChecked.Value)
				{
					element.Mapped = null;
					m_Ignores.Add(element.Name);
				}
				else
				{
					m_Ignores.Remove(element.Name);
					element.Mapped = false;
				}

				try
				{
					ListBox_ElemsToMap.SelectedItem = ElementsToMap.FirstOrDefault(el => el.Mapped == false);
					if (ListBox_ElemsToMap.SelectedItem != null) 
						ListBox_ElemsToMap.ScrollIntoView(ListBox_ElemsToMap.SelectedItem);
				}
				catch
				{
					ListBox_ElemsToMap.SelectedItem = null;
				}
			}
		}
	}

	public class MappedConverter : IValueConverter
	{

		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			bool? mapped = value as bool?;
			if (mapped == null)
				return Brushes.Red;
			return mapped.Value ? Brushes.LightGreen : null;
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if ((value as Brush) == Brushes.Red)
				return null;
			return ((value as Brush) == Brushes.LightGreen);
		}
	}
}

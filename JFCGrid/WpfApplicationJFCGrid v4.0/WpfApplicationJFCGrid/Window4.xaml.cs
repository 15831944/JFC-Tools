using System;
using System.Collections.Generic;
using System.ComponentModel;
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
using JFCGridControl;

namespace WpfApplicationJFCGrid
{
    /// <summary>
    /// Interaction logic for Window4.xaml
    /// </summary>
    public partial class Window4 : Window
    {
        public Window4()
        {
            InitializeComponent();

            toto t = new toto();

            this.DataContext = t;

            MyGrid.PropertyChildrenName = "Children";
            MyGrid.PropertyExpandeChildrenName = "IsExpande";

            BuildColumns();
            BuildData();
        }

        public void BuildColumns()
        {

            JFCGridColumn col1 = new JFCGridColumn("Nom");
            //col1.Header.Content = "nom";
            col1.Frozen = JFCGridColumn.FrozenType.Start;
            col1.LevelGrouping = 0;
            col1.TypeColumn = JFCGridColumn.TypesColumn.GroupingWithRow;
            //col1.Header.Style = this.FindResource("JFCGridColumnHeader") as Style;
            
            MyGrid.ColunmDefinition.Add(col1);

            JFCGridColumn col2 = new JFCGridColumn("Nom");
            col2.Header.Content = "nom";
            col2.Frozen = JFCGridColumn.FrozenType.Start;
            col2.LevelGrouping = 1;
            col2.TypeColumn = JFCGridColumn.TypesColumn.Hierarchical;
            //col2.Header.Style = this.FindResource("JFCGridColumnHeader") as Style;
            //col2.CellTemplate = this.FindResource("ColumnFrozenCellTemplate") as ControlTemplate;

            MyGrid.ColunmDefinition.Add(col2);


            JFCGridColumn colb1 = new JFCGridColumn("Nom");
            colb1.Header.Content = "nom";
            //colb1.Header.Style = this.FindResource("JFCGridColumnHeader") as Style;
            //colb1.CellTemplate = this.FindResource("toto") as ControlTemplate;

            MyGrid.ColunmDefinition.Add(colb1);

            //JFCGridColumn colb2 = new JFCGridColumn("Prenom");
            //colb2.Header.Content = "prenom";
            //colb2.Header.Style = this.FindResource("JFCGridColumnHeader") as Style;
            //colb2.CellTemplate = this.FindResource("toto") as ControlTemplate;

            //MyGrid.ColunmDefinition.Add(colb2);

            for (int i = 0; i < 20; i++)
            {
                JFCGridColumn colb3 = new JFCGridColumn("Age");
                colb3.Header.Content = "age";
                //colb3.Header.Style = this.FindResource("JFCGridColumnHeader") as Style;
                //colb3.CellTemplate = this.FindResource("toto") as ControlTemplate;
                MyGrid.ColunmDefinition.Add(colb3);
            }


            MyGrid.ColunmDefinition.CollectionChanged += ColunmDefinition_CollectionChanged;
        }

        private void ColunmDefinition_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            int i = 0;
        }

        List<Person> lstpers = new List<Person>();
        public void BuildData()
        {
            //List<Person> lstpers = new List<Person>();

            lstpers = new List<Person>();

            for (int i = 0; i < 10; i++)
            {
                Person persi = new Person("nom" + i, "prenom" + i, i);
                persi.IsExpande = true;

                for (int j = 0; j < 10; j++)
                {
                    Person persj = new Person("nom" + i + j, "prenom" + i + j, j);
                    persj.IsExpande = true;

                    for (int k = 0; k < 10; k++)
                    {
                        Person persk = new Person("nom" + i + j + k, "prenom" + i + j + k, k);
                        persk.IsExpande = true;

                        persj.Children.Add(persk);
                    }

                    persi.Children.Add(persj);
                }

                lstpers.Add(persi);
            }

            MyGrid.DataSource = lstpers;
        
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {

            //((toto)this.DataContext).MyProperty = new ObservableCollection<object>() { lstpers.ElementAt(3) };
            ((toto)this.DataContext).MyProperty.Add(lstpers.ElementAt(3));
        }
    }

    public class toto: INotifyPropertyChanged
    {
        private ObservableCollection<object> myProperty = new ObservableCollection<object>();
        public ObservableCollection<object> MyProperty
        {
            get { return myProperty; }
            set
            {
                myProperty = value;
                OnPropertyChanged();
            }
        }


        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}

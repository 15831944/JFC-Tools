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
using JFCGridControl;
using System.IO;
using System.ComponentModel;

namespace WpfApplicationJFCGrid
{
    /// <summary>
    /// Interaction logic for Window3.xaml
    /// </summary>
    public partial class Window3 : Window
    {
        public class Produit : INotifyPropertyChanged
        {
            public string Date { get; set; }
            public string Type { get; set; }
            public string Id { get; set; }

            private string name = "";
            public string Name 
            {
                get 
                {
                    System.Threading.Thread.Sleep(100);
                    return name;
                }
                set 
                {
                    name = value;
                } 
            }

            public Produit()
            {

            }

            public Produit(string Date, string Type, string Id, string Name, bool IsExpande)
            {
                this.Date = Date;
                this.Type = Type;
                this.Id = Id;
                this.Name = Name;
                this.IsExpande = IsExpande;
            }

            private bool isExpande = false;
            public bool IsExpande
            {
                get { return isExpande; }
                set
                {
                    if (isExpande != value)
                    {
                        isExpande = value;
                        OnPropertyChanged(new PropertyChangedEventArgs("IsExpande"));
                    }
                }
            }

            private List<Produit> children = new List<Produit>();

            public List<Produit> Children
            {
                get { return children; }
            }

            private string[] tab = new string[] { "toto","titi","tata" };

            public string[] Tab
            {
                get { return tab; }
            }


            #region INotifyPropertyChanged Members

            public event PropertyChangedEventHandler PropertyChanged;
            internal void OnPropertyChanged(PropertyChangedEventArgs e)
            {
                if (PropertyChanged != null)
                    PropertyChanged(this, e);
            }

            #endregion
        }

        private List<Produit> lstProduit;

        public Window3()
        {
            InitializeComponent();

            MyGrid.PropertyChildrenName = "Children";
            MyGrid.PropertyExpandeChildrenName = "IsExpande";
            MyGrid.SelectionMode = SelectionMode.Extended;
            MyGrid.HorizontalBorderColor = Brushes.Transparent;
            MyGrid.VerticalBorderColor = Brushes.Transparent;

            #region Chargement
            string path = @".\test.csv";
            try
            {
                if (File.Exists(path))
                {
                    lstProduit = new List<Produit>();

                    string str;
                    string pfamille = "";
                    string pclasse = "";
                    string pgroupe = "";
                    string pvariete = "";
                    string pproduit = "";

                    Produit fam = null;
                    Produit cla = null;
                    Produit grp = null;
                    Produit var = null;
                    Produit pro = null;

                    using (StreamReader sr = new StreamReader(path))
                    {
                        while (sr.Peek() >= 0)
                        {
                            str = sr.ReadLine();

                            var tab = str.Split(',');

                            if (tab.Count() == 5)
                            {
                                //SECTOR,SUBSECTOR,GROUP_,SEGMENT,PRODUCT

                                var famille = tab[0];
                                var classe = tab[1];
                                var groupe = tab[2];
                                var variete = tab[3];
                                var produit = tab[4];


                                if (pfamille != famille)
                                {
                                    fam = new Produit("01/01/2013", "Famille", "1", famille, false);

                                    lstProduit.Add(fam);

                                    pfamille = famille;
                                }

                                if (pclasse != classe)
                                {
                                    cla = new Produit("02/01/2013", "Classe", "2", classe, false);

                                    fam.Children.Add(cla);

                                    pclasse = classe;
                                }

                                if (pgroupe != groupe)
                                {
                                    grp = new Produit("03/01/2013", "Groupe", "3", groupe, false);

                                    cla.Children.Add(grp);

                                    pgroupe = groupe;
                                }

                                if (pvariete != variete)
                                {
                                    var = new Produit("04/01/2013", "Variete", "4", variete, false);

                                    grp.Children.Add(var);

                                    pvariete = variete;
                                }

                                if (pproduit != produit)
                                {
                                    pro = new Produit("05/01/2013", "Produit", "5", produit, false);

                                    var.Children.Add(pro);

                                    pproduit = produit;
                                }

                            }
                        }
                    }
                }
            }
            catch (Exception et)
            {
                Console.WriteLine("The process failed: {0}", et.ToString());
            }
            #endregion


            //JFCGridColumn Col1 = new JFCGridColumn("Date");
            ////Col1.Frozen = true;
            //Col1.TypeColumn = JFCGridColumn.TypesColumn.Normal;
            ////Col1.LevelGrouping = 0;
            ////Col1.CellTemplate = (ControlTemplate)Resources["CellTemplate2"];
            //Col1.Header.Content = "Date";
            //MyGrid.ColunmDefinition.Add(Col1);

            JFCGridColumn Col21 = new JFCGridColumn("Name");
            //Col21.Frozen = true;
            Col21.LevelGrouping = 0;
            Col21.CellTemplate = (ControlTemplate)Resources["CellTemplate2"];
            Col21.TypeColumn = JFCGridColumn.TypesColumn.GroupingWithRow;
            Col21.Header.Content = "Famille"; 
            //MyGrid.ColunmDefinition.Add(Col21);

            JFCGridColumn Col2 = new JFCGridColumn("Name");
            //Col2.Frozen = true;
            Col2.LevelGrouping = 1;
            Col2.TypeColumn = JFCGridColumn.TypesColumn.GroupingWithRow;
            Col2.CellTemplate = (ControlTemplate)Resources["CellTemplate2"];
            Col2.Header.Content = "Classe";
            //MyGrid.ColunmDefinition.Add(Col2);

            //JFCGridColumn Col22 = new JFCGridColumn("Type");
            ////Col22.Frozen = true;
            //Col22.TypeColumn = JFCGridColumn.TypesColumn.Hierarchical;
            //MyGrid.ColunmDefinition.Add(Col22);

            //JFCGridColumn Col3 = new JFCGridColumn("Id");
            ////Col3.Frozen = true;
            //Col3.TypeColumn = JFCGridColumn.TypesColumn.Normal;
            //Col3.Header.Content = "Id";
            //MyGrid.ColunmDefinition.Add(Col3);

            JFCGridColumn Col4 = new JFCGridColumn("Name");
            //Col4.Frozen = true;
            Col4.TypeColumn = JFCGridColumn.TypesColumn.Normal;
            Col4.Header.Content = "Produit";
            MyGrid.ColunmDefinition.Add(Col4);

            //JFCGridColumn Col5 = new JFCGridColumn("Tab[1]");
            ////Col4.Frozen = true;
            //Col5.TypeColumn = JFCGridColumn.TypesColumn.Normal;
            //Col5.Header.Content = "Tab";
            //MyGrid.ColunmDefinition.Add(Col5);


            MyGrid.DataSource = lstProduit;
        }
    }
}

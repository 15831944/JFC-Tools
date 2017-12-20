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
using System.Windows.Navigation;
using System.Windows.Shapes;
using JFCGridControl;
using System.IO;
using System.ComponentModel;

namespace WpfApplicationJFCGrid
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Window1 : Window
    {
        List<Person> MyList = new List<Person>();

        List<Person> MyListFooter = new List<Person>();

        JFCGridColumn colh;

        public Window1()
        {
            InitializeComponent();

            MyGrid.VerticalBorderColor = new SolidColorBrush(Brushes.Pink.Color);
            MyGrid.HorizontalBorderColor = new SolidColorBrush(Brushes.Violet.Color);

            int renderingTier = (RenderCapability.Tier >> 16);

            this.Closed += new EventHandler(Window1_Closed);

            //MyGrid.IsVirtualised = false;

            BuilColumns();

            for (int i = 0; i < 10; i++)
            {
                ////MyList.Add(new Person("Chambard", "Alain", 100));
                ////MyList.Add(new Person("Benesteau", "Stephane", 40));
                ////MyList.Add(new Person("Solau", "Xavier", 35));
                ////MyList.Add(new Person("Legeay", "Frederic", 18));

                ////MyList.Add(new Person("Nom" + i.ToString(), "Prenom" + i.ToString(), i));

                //Person pers = new Person("Nom" + i.ToString(), "Prenom" + i.ToString(), i);

                ////if (i % 2 == 0)
                ////    pers.IsExpande = true;

                ////pers.Children.Add(new Person("Chambard", "Alain", 100));

                //Person p = new Person("Benesteau", "Stephane", 40);

                ////for (int j = 0; j < 100; j++)
                ////{
                ////    p.Children.Add(new Person("Nom" + j.ToString(), "Prenom" + j.ToString(), j));
                ////}

                //pers.Children.Add(p);
                ////pers.Children.Add(new Person("Benesteau", "Stephane", 40));
                //pers.Children.Add(new Person("Solau", "Xavier", 35, false));
                //pers.Children.Add(new Person("Legeay", "Frederic", 18, true));

                //MyList.Add(pers);

                Person pi = new Person("n" + i.ToString(), "p" + i.ToString(), i);
                pi.IsExpande = true;

                for (int j = 0; j < 10; j++)
                {
                    Person pj = new Person(pi.Nom + j.ToString(), pi.Prenom + j.ToString(), j);
                    pj.IsExpande = true;

                    pi.Children.Add(pj);

                    //for (int k = 0; k < 10; k++)
                    //{
                    //    Person pk = new Person(pj.Nom + k.ToString(), pj.Prenom + k.ToString(), k);
                    //    pk.IsExpande = true;

                    //    pj.Children.Add(pk);

                    //    for (int l = 0; l < 10; l++)
                    //    {
                    //        Person pl = new Person(pk.Nom + l.ToString(), pk.Prenom + l.ToString(), l);
                    //        pl.IsExpande = true;

                    //        pk.Children.Add(pl);

                    //        for (int m = 0; m < 10; m++)
                    //        {
                    //            Person pm = new Person(pl.Nom + m.ToString(), pl.Prenom + m.ToString(), m);
                    //            pm.IsExpande = true;

                    //            pl.Children.Add(pm);
                    //        }
                    //    }
                    //}
                }

                MyList.Add(pi);

            }

            //MyTreeView.DataContext = MyList;

            MyGrid.PropertyChildrenName = "Children";
            MyGrid.PropertyExpandeChildrenName = "IsExpande";
            MyGrid.DataSource = MyList;

            MyGrid.DataSourceFooter = new List<Person>() { new Person("test","test",15) };
                        
            //Binding binding = new Binding("IsExpande");
            //binding.Source = MyList[0];

            //MyTextBlock.SetBinding(TextBlock.TextProperty, binding);

            MyGrid.SelectionMode = SelectionMode.Extended;

            //ContextMenu contextMenu = new ContextMenu();

            //MenuItem m = new MenuItem();
            //m.Header = "test";

            //contextMenu.Items.Add(m);

            //MyGrid.ContextmenuRow = contextMenu;

            //MyGrid.LineOverChanged += new DependencyPropertyChangedEventHandler(MyGrid_LineOverChanged);

            //MyGrid.LineOverChanged +=new JFCGrid.DependencyPropertyLineOverChangedEventHandler(MyGrid_LineOverChanged);

            //MyGrid.LineOverChanged += new JFCGrid.DependencyPropertyLineOverChangedEventHandler(MyGrid_LineOverChanged);


            //JFCGridColumn ColF = new JFCGridColumn("Prenom");
            ////ColF.Frozen = true;
            //ColF.Width = new GridLength(1, GridUnitType.Star);
            //MyGrid.ColunmDefinitionFooter.Add(ColF);


            //MyListFooter.Add(new Person("Chambard", "Alain", 100));
            ////MyListFooter.Add(new Person("Benesteau", "Stephane", 40));
            ////MyListFooter.Add(new Person("Solau", "Xavier", 35));
            ////MyListFooter.Add(new Person("Legeay", "Frederic", 18));

            //MyGrid.DataSourceFooter = MyListFooter;

            MyGrid.ScrollViewerBorderThickness = new Thickness(1);
            MyGrid.ScrollViewerBorderColor = Brushes.Black;



            //MyGrid.IsSearchVisible = true;

            //List<JFCGraph.Serie> lstSerie = new List<JFCGraph.Serie>();

            //JFCGraph.Serie serie = new JFCGraph.Serie();

            //serie.Name = "toto";
            //serie.Color = Colors.Blue;

            //List<double> pts = new List<double>();

            //for (int i = 0; i < 10; i++)
            //    pts.Add(i*4);

            //serie.Points = pts;

            //lstSerie.Add(serie);

            //MyGraph.BeforeSpaceBar = 1;
            //MyGraph.AfterSpaceBar = 1;
            //MyGraph.Series = lstSerie;

        }

        void MyGrid_SearchChanged(object sender, JFCGrid.DependencyPropertySearchChangedEventArgs e)
        {
            int i = 0;
        }

        public void BuilColumns()
        {

            JFCGridColumn Col20 = new JFCGridColumn("Prenom");
            Col20.Frozen = JFCGridColumn.FrozenType.Start;
            Col20.TypeColumn = JFCGridColumn.TypesColumn.GroupingWithRow;
            Col20.LevelGrouping = 0;
            //Col20.CellTemplate = (ControlTemplate)Resources["CellTemplate2"];
            MyGrid.ColunmDefinition.Add(Col20);

            JFCGridColumn col32 = new JFCGridColumn("Prenom");
            col32.Frozen = JFCGridColumn.FrozenType.Start;
            col32.TypeColumn = JFCGridColumn.TypesColumn.GroupingWithRow;
            col32.LevelGrouping = 1;
            MyGrid.ColunmDefinition.Add(col32);


            JFCGridColumn Col21 = new JFCGridColumn("Prenom");
            Col21.Frozen = JFCGridColumn.FrozenType.Start;

            //Col21.CellTemplate = (ControlTemplate)Resources["ColumnSupportCellTemplate"];

            //Col21.IsHierarchicalColumn = true;
            Col21.TypeColumn = JFCGridColumn.TypesColumn.Hierarchical;
            //Col21.Width = new GridLength(50);
            MyGrid.ColunmDefinition.Add(Col21);


            //JFCGridColumn c = new JFCGridColumn("1");
            //c.Width = new GridLength(1, GridUnitType.Star);
            //MyGrid.ColunmDefinition.Add(c);


            JFCGridColumn colB1 = new JFCGridColumn("Prenom");
            colB1.Width = new GridLength(50);
            MyGrid.ColunmDefinition.Add(colB1);
            JFCGridColumn colB2 = new JFCGridColumn("Prenom");
            colB2.Width = new GridLength(50);
            MyGrid.ColunmDefinition.Add(colB2);
            JFCGridColumn colB3 = new JFCGridColumn("Prenom");
            colB3.Width = new GridLength(50);
            MyGrid.ColunmDefinition.Add(colB3);
            JFCGridColumn colB4 = new JFCGridColumn("Nom");
            colB4.Width = new GridLength(100);
            MyGrid.ColunmDefinition.Add(colB4);



            //JFCGridColumn colFoot = new JFCGridColumn();
            ////colFoot.CellTemplate = (ControlTemplate)this.Resources["FooterLibelleTemplateKey"];
            //colFoot.Width = new GridLength(1, GridUnitType.Star);
            //MyGrid.ColunmDefinitionFooter.Add(colFoot);

            //JFCGridColumn Col25 = new JFCGridColumn("Prenom");
            //Col25.TypeColumn = JFCGridColumn.TypesColumn.GroupingNoRow;
            //Col25.LevelGrouping = 0;
            //MyGrid.ColunmDefinition.Add(Col25);

            //JFCGridColumn col33 = new JFCGridColumn("Prenom");
            //col33.TypeColumn = JFCGridColumn.TypesColumn.GroupingNoRow;
            //col33.LevelGrouping = 1;
            //MyGrid.ColunmDefinition.Add(col33);

            //List<JFCGridColumn> lstCol = new List<JFCGridColumn>();


            //for (int i = 0; i < 1; i++)
            //{
            //    //JFCGridColumn Col00 = new JFCGridColumn();
            //    //Col00.CellTemplate = (ControlTemplate)this.FindResource("SegmentTemplateKey");
            //    ////Col00.IsHierarchicalColumn = true;
            //    ////MyGrid.ColunmDefinition.Add(Col00);
            //    //lstCol.Add(Col00);

            //    JFCGridColumn Col15 = new JFCGridColumn("Age");
            //    //Col15.Width = new GridLength(50);
            //    Col15.Header.Content = "Col15";


            //    JFCGridColumn Col10 = new JFCGridColumn("Prenom");

            //    //TextBlock textblock2 = new TextBlock();
            //    //textblock2.Text = "toto titi tata tutu 2";
            //    //RotateTransform rt2 = new RotateTransform(-45);
            //    //textblock2.LayoutTransform = rt2;

            //    Col10.Header.Content = "Prenom";

            //    //Col10.Width = new GridLength(50);
            //    JFCGridColumn Col11 = new JFCGridColumn("Nom");
            //    //Col11.Width = new GridLength(50);

            //    Col15.ChildrenColumns.Add(Col10);
            //    Col15.ChildrenColumns.Add(Col11);
            //    //MyGrid.ColunmDefinition.Add(Col15);
            //    lstCol.Add(Col15);

            //    colh = Col15;


            //    JFCGridColumn Col22 = new JFCGridColumn("Age");
            //    //Col22.Frozen = true;
            //    //Col22.Width = new GridLength(50);
            //    Col22.Header.Content = "Age";
            //    //Col22.CellTemplate = (ControlTemplate)this.FindResource("CellTemplate");
            //    //MyGrid.ColunmDefinition.Add(Col22);
            //    lstCol.Add(Col22);

            //    JFCGridColumn Col1 = new JFCGridColumn("Nom");
            //    //Col1.Width = new GridLength(50);
            //    Col1.CellStyle.TextAlignment = TextAlignment.Right;
            //    //Col1.CellTemplate = (ControlTemplate)this.FindResource("CellTemplate");
            //    Col1.Header.Content = "Parent";
            //    //MyGrid.ColunmDefinition.Add(Col1);
            //    lstCol.Add(Col1);

            //    JFCGridColumn Col2 = new JFCGridColumn("Prenom");
            //    //Col2.Width = new GridLength(50);
            //    //Col2.IsHierarchicalColumn = true;
            //    //MyGrid.ColunmDefinition.Add(Col2);
            //    //Col2.CellTemplate = (ControlTemplate)this.FindResource("CellTemplate");
            //    lstCol.Add(Col2);

            //    JFCGridColumn Col3 = new JFCGridColumn("Age");
            //    //Col3.Width = new GridLength(50);
            //    Col3.CellStyle.TextAlignment = TextAlignment.Right;
            //    //Col3.CellTemplate = (ControlTemplate)this.FindResource("CellTemplate");
            //    //MyGrid.ColunmDefinition.Add(Col3);
            //    lstCol.Add(Col3);

            //    JFCGridColumn Col4 = new JFCGridColumn("Nom");
            //    //Col4.Width = new GridLength(50);
            //    //MyGrid.ColunmDefinition.Add(Col4);
            //    lstCol.Add(Col4);

            //    JFCGridColumn Col5 = new JFCGridColumn("Prenom");
            //    //Col5.Width = new GridLength(50);
            //    //MyGrid.ColunmDefinition.Add(Col5);
            //    lstCol.Add(Col5);

            //    JFCGridColumn Col6 = new JFCGridColumn("Age");
            //    //Col6.Width = new GridLength(50);
            //    //MyGrid.ColunmDefinition.Add(Col6);
            //    lstCol.Add(Col6);

            //    JFCGridColumn Col7 = new JFCGridColumn("Nom");
            //    //Col7.Width = new GridLength(50);
            //    //MyGrid.ColunmDefinition.Add(Col7);
            //    lstCol.Add(Col7);




            //    ////for (int t = 0; t < 500; t++)
            //    ////{ 
            //    //JFCGridColumn Col8 = new JFCGridColumn("Prenom");
            //    ////Col8.Width = new GridLength(50);
            //    //MyGrid.ColunmDefinition.Add(Col8);


            //    //JFCGridColumn Col9 = new JFCGridColumn("Age");
            //    ////Col9.Width = new GridLength(50);
            //    //MyGrid.ColunmDefinition.Add(Col9);
            //    ////}

            //    //JFCGridColumn Col15 = new JFCGridColumn("Age");
            //    ////Col15.Width = new GridLength(50);
            //    //Col15.Header.Content = "Col15";


            //    //JFCGridColumn Col10 = new JFCGridColumn("Prenom");

            //    //TextBlock textblock2 = new TextBlock();
            //    //textblock2.Text = "toto titi tata tutu 2";
            //    //RotateTransform rt2 = new RotateTransform(-45);
            //    //textblock2.LayoutTransform = rt2;

            //    //Col10.Header.Content = textblock2;

            //    ////Col10.Width = new GridLength(50);
            //    //JFCGridColumn Col11 = new JFCGridColumn("Nom");
            //    ////Col11.Width = new GridLength(50);

            //    //Col15.ChildrenColumns.Add(Col10);
            //    //Col15.ChildrenColumns.Add(Col11);
            //    ////MyGrid.ColunmDefinition.Add(Col15);
            //    //lstCol.Add(Col15);

            //    //colh = Col15;

            //    JFCGridColumn Col12 = new JFCGridColumn("Age");
            //    Col12.Header.Content = "Col12";

            //    JFCGridColumn Col13 = new JFCGridColumn("Nom");
            //    Col13.Header.Visibility = System.Windows.Visibility.Collapsed;
            //    //Col13.Width = new GridLength(50);
            //    JFCGridColumn Col14 = new JFCGridColumn("Prenom");
            //    //Col14.Width = new GridLength(50);

            //    Col12.ChildrenColumns.Add(Col13);
            //    Col12.ChildrenColumns.Add(Col14);

            //    //MyGrid.ColunmDefinition.Add(Col12);
            //    lstCol.Add(Col12);

            //}

            //MyGrid.ColunmDefinition.AddRange(lstCol);
        }


        private void SuppImg_MouseEnter(object sender, MouseEventArgs e)
        {
            Image img = sender as Image;

            if (img != null)
            {
                ToolTip tt = new System.Windows.Controls.ToolTip();
                tt.Content = TxtTooltip;
                img.ToolTip = tt;

                tt.IsOpen = true;
            }
        }

        private void SuppImg_MouseLeave(object sender, MouseEventArgs e)
        {
            Image img = sender as Image;

            if (img != null)
            {
                ToolTip tt = img.ToolTip as ToolTip;

                if (tt != null)
                    tt.IsOpen = false;
            }
        }

        private void SuppImg_IsVisibleChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            Border img = sender as Border;

            //if (TxtTooltip == null)
            //    //img.Visibility = System.Windows.Visibility.Hidden;
            //    img.Opacity = 0;


        }

        void cell_IsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {

        }

        void Window1_Closed(object sender, EventArgs e)
        {
            //MessageBox.Show("fermeture");
        }

        //void MyGrid_LineOverChanged(object sender, DependencyPropertyChangedEventArgs e)
        //{
        //    Person p = e.NewValue as Person;

        //    if (p != null)
        //        LineOver.Text = p.Nom;
        //    else
        //        LineOver.Text = "";
        //}

        public string TxtTooltip = null;
        public bool IsTxtTooltip = false;

        void MyGrid_LineOverChanged(object sender, JFCGridControl.JFCGrid.DependencyPropertyLineOverChangedEventArgs e)
        {
            Person p = e.NewValue as Person;

            //if (p != null)
            //    LineOver.Text = e.IndexDataRow.ToString();
            //else
            //    LineOver.Text = "";

            TxtTooltip = null;
            IsTxtTooltip = false;

            if (p != null)
                if ((p.Age % 3) == 0)
                {
                    TxtTooltip = p.ToString();
                    IsTxtTooltip = true;
                }

        }

        //public class Person
        //{

        //    public Person()
        //    {

        //    }

        //    public Person(string Nom, string Prenom, int Age)
        //    {
        //        this.Nom = Nom;
        //        this.Prenom = Prenom;
        //        this.Age = Age;

        //    }

        //    public string Nom { get; set; }
        //    public string Prenom { get; set; }
        //    public int Age { get; set; }
        //    public string[] text
        //    {
        //        get { return new string[2] { "fred", "alain" }; }
        //    }

        //    public List<Person> Children = new List<Person>();
        //}

        JFCGridColumn Col30;

        //private void MyButton_Click(object sender, RoutedEventArgs e)
        //{
        //    Col30 = new JFCGridColumn("Age");
        //    Col30.Width = new GridLength(100);
        //    //Col30.CellStyle.BackColor = Brushes.Red;
        //    //MyGrid.ColunmDefinition.Add(Col30);
        //    MyGrid.ColunmDefinition.Insert(10,Col30);
        //}

        private void MyButton1_Click(object sender, RoutedEventArgs e)
        {
            ////int index = MyGrid.ColunmDefinition.IndexOf(Col30);
            ////MyGrid.ColunmDefinition.Move(index, index - 1);

            //MyButton.Visibility = Visibility.Visible;

            //TextBlock textblock2 = new TextBlock();
            //textblock2.Text = "toto titi tata tutu 2";
            //RotateTransform rt2 = new RotateTransform(-45);
            //textblock2.LayoutTransform = rt2;

            //MyGrid.ColunmDefinition[10].Header.Content = textblock2;


            //for (int i = 0; i < 6154; i++)
            //{
            //    //MyList.Add(new Person("Chambard", "Alain", 100));
            //    //MyList.Add(new Person("Benesteau", "Stephane", 40));
            //    //MyList.Add(new Person("Solau", "Xavier", 35));
            //    //MyList.Add(new Person("Legeay", "Frederic", 18));

            //    MyList.Add(new Person("Nom" + i.ToString(), "Prenom" + i.ToString(), i));
            //}

            //MyGrid.DataSource = MyList;


            //JFCGridColumn ColF = new JFCGridColumn("Prenom");
            ////ColF.Frozen = true;
            ////Col21.Width = new GridLength(50);
            ////MyGrid.ColunmDefinitionFooter.Add(ColF);

            //MyListFooter.Clear();
            //MyListFooter.Add(new Person("Chambard", "Alain", DateTime.Now.Second));
            //MyListFooter.Add(new Person("Benesteau", "Stephane", 40));
            //MyListFooter.Add(new Person("Solau", "Xavier", 35));
            ////MyListFooter.Add(new Person("Legeay", "Frederic", 18));

            //MyGrid.DataSourceFooter = MyListFooter;


            /////////////////////////////////////////////////
            //JFCGridColumn col1 = new JFCGridColumn("Nom");
            //JFCGridColumn col2 = new JFCGridColumn("Nom");

            //List<JFCGridColumn> lst = new List<JFCGridColumn>();

            //lst.Add(col1);
            //lst.Add(col2);

            //MyGrid.ColunmDefinition.AddRange(lst);


            //MyGrid.ColunmDefinition.Clear();
            //BuilColumns();

            //MyCellImg2.Text = DateTime.Now.ToString();


            //if (MyList[0].IsExpande == true)
            //    MyList[0].IsExpande = false;
            //else
            //    MyList[0].IsExpande = true;



            //JFCGridColumn c1 = new JFCGridColumn("Nom");
            //c1.Header = new JFCGridColumnHeader();

            //JFCGridColumn c2 = new JFCGridColumn("Prenom");
            //c2.Header = new JFCGridColumnHeader();

            //MyGrid.ColunmDefinition[0].ChildrenColumns.Add(c1);
            //MyGrid.ColunmDefinition[0].ChildrenColumns.Add(c2);


            //MyGrid.ColunmDefinition.Clear();



            //JFCGridColumn c1 = new JFCGridColumn("Nom");
            //c1.Header = new JFCGridColumnHeader();
            //c1.Header.Content = "C1";

            //JFCGridColumn c2 = new JFCGridColumn("Prenom");
            //c2.Header = new JFCGridColumnHeader();
            //c2.Header.Content = "C2";

            //MyGrid.ColunmDefinition[3].ChildrenColumns.Add(c1);
            //MyGrid.ColunmDefinition[3].ChildrenColumns.Add(c2);


            //List<JFCGridColumn> lst = new List<JFCGridColumn>();

            //lst.Add(MyGrid.ColunmDefinition[3]);
            //lst.Add(MyGrid.ColunmDefinition[4]);
            //lst.Add(MyGrid.ColunmDefinition[5]);

            //MyGrid.ColunmDefinition.RemoveRange(lst);

            //if (MyGrid.ColunmDefinition[0].IsHierarchicalColumn == true)
            //{
            //    MyGrid.PropertyChildrenName = "";
            //    MyGrid.ColunmDefinition[0].IsHierarchicalColumn = false;                
            //}
            //else
            //{
            //    MyGrid.PropertyChildrenName = "Children";
            //    MyGrid.ColunmDefinition[0].IsHierarchicalColumn = true;                
            //}

            //if (MyGrid.ColunmDefinition[9].ChildrenColumns.Count() > 0)
            //{
            //    //MyGrid.ColunmDefinition[9].ChildrenColumns.RemoveAt(0);
            //    ////MyGrid.ColunmDefinition.RemoveAt(1);

            //    JFCGridColumn Col12 = new JFCGridColumn("Age");
            //    Col12.Header.Content = "Col12";

            //    MyGrid.ColunmDefinition[9].ChildrenColumns.Add(Col12);
            //}


            //MyGrid.IsVirtualised = !MyGrid.IsVirtualised;

            //MyGrid.PropertyChildrenName = "Children";
            //MyGrid.PropertyExpandeChildrenName = "IsExpande";


            //ContextMenu contextMenu = new ContextMenu();

            //MenuItem m = new MenuItem();
            //m.Header = DateTime.Now.Second.ToString();


            //contextMenu.Items.Add(m);

            //MyGrid.ContextmenuRow = contextMenu;

            //MyGrid.IsSearchVisible = !MyGrid.IsSearchVisible;

            //MyGrid.SearchChanged += new JFCGrid.DependencyPropertySearchChangedEventHandler(MyGrid_SearchChanged);


            BuilColumns();


            for (int i = 0; i < 50; i++)
            {

                Person pi = new Person("n" + i.ToString(), "p" + i.ToString(), i);
                pi.IsExpande = true;

                for (int j = 0; j < 3; j++)
                {
                    Person pj = new Person(pi.Nom + j.ToString(), pi.Prenom + j.ToString(), j);
                    pj.IsExpande = true;

                    pi.Children.Add(pj);

                    for (int k = 0; k < 1; k++)
                    {
                        Person pk = new Person(pj.Nom + k.ToString(), pj.Prenom + k.ToString(), k);
                        pk.IsExpande = true;

                        pj.Children.Add(pk);

                        for (int l = 0; l < 1; l++)
                        {
                            Person pl = new Person(pk.Nom + l.ToString(), pk.Prenom + l.ToString(), l);
                            pl.IsExpande = true;

                            pk.Children.Add(pl);

                            for (int m = 0; m < 1; m++)
                            {
                                Person pm = new Person(pl.Nom + m.ToString(), pl.Prenom + m.ToString(), m);
                                pm.IsExpande = true;

                                pl.Children.Add(pm);
                            }
                        }
                    }
                }

                MyList.Add(pi);

            }

            //MessageBox.Show("crétion fini");

            MyGrid.DataSource = MyList;
        }

        private void MyButton2_Click(object sender, RoutedEventArgs e)
        {
            ////int index = MyGrid.ColunmDefinition.IndexOf(Col30);
            ////MyGrid.ColunmDefinition.Move(index, index + 1);

            ////JFCGridColumn Col11 = new JFCGridColumn("Nom");
            ////Col11.Header.Content = "coucoucoucou";
            ////colh.ChildrenColumns.Add(Col11);

            //if (MyGrid.HorizontalBorderColor == Brushes.Red)
            //{
            //    MyGrid.HorizontalBorderColor = Brushes.Blue;
            //    MyGrid.VerticalBorderColor = Brushes.Red;
            //    MyGrid.ScrollViewerBorderColor = Brushes.Orange;
            //}
            //else
            //{
            //    MyGrid.HorizontalBorderColor = Brushes.Red;
            //    MyGrid.VerticalBorderColor = Brushes.Blue;
            //    MyGrid.ScrollViewerBorderColor = Brushes.OrangeRed;
            //}

            ////if (MyGrid.ScrollBarVerticalVisibility != Visibility.Visible)
            ////    MyGrid.ScrollBarVerticalVisibility = Visibility.Visible;
            ////else
            ////    MyGrid.ScrollBarVerticalVisibility = Visibility.Collapsed;

            ///MyGrid.ColunmDefinition.ElementAt(1).ActualWidth = new GridLength(0);
            ///


            //Object obj = MyGrid.SelectedItems[0] as Object;

            //if (MyList[0].Children[1].IsExpande == true)
            //    MyList[0].Children[1].IsExpande = false;
            //else
            //    MyList[0].Children[1].IsExpande = true;


            //BuilColumns();

            //int count = MyGrid.ColunmDefinition[3].ChildrenColumns.Count;

            //for (int i = 0; i < count;i++ )
            //    MyGrid.ColunmDefinition[3].ChildrenColumns.RemoveAt(0);

            MyGrid.SearchChanged -= MyGrid_SearchChanged;
        }

        public class Produit : INotifyPropertyChanged
        {
            public string Date { get; set; }
            public string Type { get; set; }
            public string Id { get; set; }
            public string Name { get; set; }

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

        private void MyButtonChargement_Click(object sender, RoutedEventArgs e)
        {
            //D:\visual studio 2010\Projects\WpfApplicationJFCGrid Dev

            //string path = @"D:\visual studio 2010\Projects\WpfApplicationJFCGrid Dev\test.csv";
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
                                    fam = new Produit("01/01/2013", "Famille", "1", famille, true);

                                    lstProduit.Add(fam);

                                    pfamille = famille;
                                }

                                if (pclasse != classe)
                                {
                                    cla = new Produit("02/01/2013", "Classe", "2", classe, true);

                                    fam.Children.Add(cla);

                                    pclasse = classe;
                                }

                                if (pgroupe != groupe)
                                {
                                    grp = new Produit("03/01/2013", "Groupe", "3", groupe, true);

                                    cla.Children.Add(grp);

                                    pgroupe = groupe;
                                }

                                if (pvariete != variete)
                                {
                                    var = new Produit("04/01/2013", "Variete", "4", variete, true);

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

            MessageBox.Show("Chargement terminé");
        }

        private void MyButtonAffichage_Click(object sender, RoutedEventArgs e)
        {

            JFCGridColumn Col1 = new JFCGridColumn("Date");
            Col1.Frozen = JFCGridColumn.FrozenType.Start;
            Col1.TypeColumn = JFCGridColumn.TypesColumn.Normal;
            //Col1.LevelGrouping = 0;
            //Col1.CellTemplate = (ControlTemplate)Resources["CellTemplate2"];
            MyGrid.ColunmDefinition.Add(Col1);

            JFCGridColumn Col21 = new JFCGridColumn("Name");
            Col21.Frozen = JFCGridColumn.FrozenType.Start;
            Col21.LevelGrouping = 0;
            Col21.CellTemplate = (ControlTemplate)Resources["CellTemplate2"];
            Col21.TypeColumn = JFCGridColumn.TypesColumn.GroupingWithRow;
            MyGrid.ColunmDefinition.Add(Col21);

            //JFCGridColumn Col2 = new JFCGridColumn("Type");
            //Col2.Frozen = true;
            //Col2.TypeColumn = JFCGridColumn.TypesColumn.Hierarchical;
            //MyGrid.ColunmDefinition.Add(Col2);

            JFCGridColumn Col2 = new JFCGridColumn("Name");
            Col2.Frozen = JFCGridColumn.FrozenType.Start;
            Col2.LevelGrouping = 1;
            Col2.TypeColumn = JFCGridColumn.TypesColumn.GroupingWithRow;
            Col2.CellTemplate = (ControlTemplate)Resources["CellTemplate2"];
            MyGrid.ColunmDefinition.Add(Col2);

            JFCGridColumn Col22 = new JFCGridColumn("Type");
            Col22.Frozen = JFCGridColumn.FrozenType.Start;
            Col22.TypeColumn = JFCGridColumn.TypesColumn.Hierarchical;
            MyGrid.ColunmDefinition.Add(Col22);

            JFCGridColumn Col3 = new JFCGridColumn("Id");
            Col3.Frozen = JFCGridColumn.FrozenType.Start;
            Col3.TypeColumn = JFCGridColumn.TypesColumn.Normal;
            MyGrid.ColunmDefinition.Add(Col3);

            JFCGridColumn Col4 = new JFCGridColumn("Name");
            Col4.Frozen = JFCGridColumn.FrozenType.Start;
            Col4.TypeColumn = JFCGridColumn.TypesColumn.Normal;
            MyGrid.ColunmDefinition.Add(Col4);


            MyGrid.DataSource = lstProduit;
        }

        private void MyButtonExpande_Click(object sender, RoutedEventArgs e)
        {
            //if (lstProduit[0].Children[0].IsExpande == false)
            //    lstProduit[0].Children[0].IsExpande = true;
            //else
            //    lstProduit[0].Children[0].IsExpande = false;

            if (lstProduit[0].IsExpande == false)
                lstProduit[0].IsExpande = true;
            else
                lstProduit[0].IsExpande = false;
        }



    }
}

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

namespace WpfApplicationJFCGrid
{
    /// <summary>
    /// Interaction logic for Window2.xaml
    /// </summary>
    public partial class Window2 : Window
    {
        public Window2()
        {
            InitializeComponent();

            List<JFCGraph.Serie> lstSerie = new List<JFCGraph.Serie>();

            JFCGraph.Serie serie = new JFCGraph.Serie();

            serie.Name = "toto";
            serie.Color = Brushes.Blue;

            List<JFCGraph.Serie.Value> pts = new List<JFCGraph.Serie.Value>();

            for (int i = 1; i < 10; i++)
                pts.Add(new JFCGraph.Serie.Value() { Point = i * 4, Label= (i*4).ToString() });

            serie.Points = pts;

            lstSerie.Add(serie);

            MyGraph.BeforeSpaceBar = 1;
            MyGraph.AfterSpaceBar = 1;
            MyGraph.BarSize = 20;
            MyGraph.Series = lstSerie;

            MyGraph.SizeRectPoint = new Size(10, 10);
        }

        private void MyButton_Click(object sender, RoutedEventArgs e)
        {


            if (MyGraph.GraphType == JFCGraph.GraphTypes.Curve)
                MyGraph.GraphType = JFCGraph.GraphTypes.Histogram;
            else
                MyGraph.GraphType = JFCGraph.GraphTypes.Curve;


        }
    }
}

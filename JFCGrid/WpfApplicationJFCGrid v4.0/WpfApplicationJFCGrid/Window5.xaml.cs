using JFCGridControl;
using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace WpfApplicationJFCGrid
{
    /// <summary>
    /// Interaction logic for Window5.xaml
    /// </summary>
    public partial class Window5 : Window
    {
        Row[] Data;

        public Window5()
        {
            InitializeComponent();
        }

        void BuildColumns()
        {

            JFCGridColumn col1 = new JFCGridColumn("Value[0].Value");
            col1.Header.Content = "test";
            MyGrid.ColunmDefinition.Add(col1);
            JFCGridColumn col2 = new JFCGridColumn("Value[1].Value");
            col2.Header.Content = "test";
            MyGrid.ColunmDefinition.Add(col2);
            JFCGridColumn col3 = new JFCGridColumn("Value[2].Value");
            col3.Header.Content = "test";
            MyGrid.ColunmDefinition.Add(col3);
            
        }

        void CreatData()
        {
            Data = new Row[5];

            Row row = null;

            for (int i = 0; i < 5; i++)
            {
                row = new Row();

                row.Value = new MyInt[3];

                for (int j = 0; j < 3; j++)
                {
                    row.Value[j] = new MyInt();
                    row.Value[j].Value = j;
                }

                Data[i] = row;
            }
        }

        private void ButtonLoad_Click(object sender, RoutedEventArgs e)
        {
            BuildColumns();

            CreatData();

            MyGrid.DataSource = Data;
        }

        private void ButtonChangeData_Click(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < 5; i++)
            {
                //Data[i].Value = new object[3];

                for (int j = 0; j < 3; j++)
                {
                    Data[i].Value[j].Value = Data[i].Value[j].Value + 1;
                }

            }
        }

        public class Row
        {
            private MyInt[] m_values;

            public MyInt[] Value
            {
                get
                { return this.m_values; }
                set
                { this.m_values = value; }
            }
        }

        public class MyInt : INotifyPropertyChanged
        {
            private int mvalue;
            public int Value
            {
                get { return mvalue; }
                set
                {
                    mvalue = value;
                    OnPropertyChanged("Value");
                }
            }

            public event PropertyChangedEventHandler PropertyChanged;

            private void OnPropertyChanged(String propertyName)
            {
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
                }
            }
        }

        private void ButtonChangeForeground_Click(object sender, RoutedEventArgs e)
        {
            CellImageTest.Foreground = Brushes.Aqua;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.ComponentModel;

namespace JFCGridControl
{
    public class JFCGridCellStyle : INotifyPropertyChanged
    {
        private TextAlignment textAlignment = TextAlignment.Left;
        public TextAlignment TextAlignment
        {
            get { return textAlignment; }
            set 
            { 
                textAlignment = value;
                OnPropertyChanged(new PropertyChangedEventArgs("TextAlignment"));
            }
        }
        
        //public Brush BackColor = null;
        
        ////public Font Font;

        //public Brush ForeColor = Brushes.Black;

        ////Format
        
        ////public string NullValue = "";

        //public Thickness Margin = new Thickness(0);
        
        ////SelectionBackColor
        ////SelectionForeColor
        ////Tag

        //public TextWrapping WrapMode = TextWrapping.NoWrap;

        //
        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }

    }
}

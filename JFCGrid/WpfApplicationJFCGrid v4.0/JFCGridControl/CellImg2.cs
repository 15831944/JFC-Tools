// JFCGridControl.CellImg2
using JFCGridControl;
using System.ComponentModel;
using System.Globalization;
using System.Windows;
using System.Windows.Media;


namespace JFCGridControl
{
    public class CellImg2 : FrameworkElement, INotifyPropertyChanged
    {
        public static readonly DependencyProperty TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(CellImg2), new UIPropertyMetadata("", CellImg2.UpdateText));

        public static readonly DependencyProperty FontFamilyProperty = DependencyProperty.Register("FontFamily", typeof(FontFamily), typeof(CellImg2), new FrameworkPropertyMetadata(SystemFonts.MessageFontFamily, FrameworkPropertyMetadataOptions.Inherits));

        public static readonly DependencyProperty FontSizeProperty = DependencyProperty.Register("FontSize", typeof(double), typeof(CellImg2), new FrameworkPropertyMetadata(SystemFonts.MessageFontSize, FrameworkPropertyMetadataOptions.Inherits));

        public static readonly DependencyProperty TextAlignmentProperty = DependencyProperty.Register("TextAlignment", typeof(TextAlignment), typeof(CellImg2), new UIPropertyMetadata(TextAlignment.Left, CellImg2.UpdateText));

        private object data;

        public string Text
        {
            get
            {
                return (string)base.GetValue(CellImg2.TextProperty);
            }
            set
            {
                base.SetValue(CellImg2.TextProperty, value);
            }
        }

        public FontFamily FontFamily
        {
            get
            {
                return (FontFamily)base.GetValue(CellImg2.FontFamilyProperty);
            }
            set
            {
                base.SetValue(CellImg2.FontFamilyProperty, value);
            }
        }

        public double FontSize
        {
            get
            {
                return (double)base.GetValue(CellImg2.FontSizeProperty);
            }
            set
            {
                base.SetValue(CellImg2.FontSizeProperty, value);
            }
        }

        public TextAlignment TextAlignment
        {
            get
            {
                return (TextAlignment)base.GetValue(CellImg2.TextAlignmentProperty);
            }
            set
            {
                base.SetValue(CellImg2.TextAlignmentProperty, value);
            }
        }

        public object Data
        {
            get
            {
                return this.data;
            }
            set
            {
                this.data = value;
                this.OnPropertyChanged(new PropertyChangedEventArgs("Data"));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private static void UpdateText(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            CellImg2 cellImg = obj as CellImg2;
            if (e.NewValue != e.OldValue)
            {
                DrawingContext drawingContext = new DrawingVisual().RenderOpen();
                cellImg.OnRender(drawingContext);
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            string text = "";
            if (this.Text != null)
            {
                text = this.Text;
                if (!(this.Text == ""))
                {
                    Point origin = new Point(0.0, 0.0);
                    FormattedText formattedText = new FormattedText(text, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(this.FontFamily.ToString()), this.FontSize, Brushes.Black);
                    if (this.TextAlignment != 0)
                    {
                        if (this.TextAlignment == TextAlignment.Right)
                        {
                            origin.X = base.ActualWidth - formattedText.Width;
                        }
                        else if (this.TextAlignment == TextAlignment.Center)
                        {
                            origin.X = (base.ActualWidth - formattedText.Width) / 2.0;
                        }
                        else
                        {
                            TextAlignment textAlignment = this.TextAlignment;
                        }
                    }
                    drawingContext.DrawText(formattedText, origin);
                }
            }
        }

        private void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, e);
            }
        }
    }
}
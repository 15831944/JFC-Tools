// JFCGridControl.CellImg
using JFCGridControl;
using System;
using System.ComponentModel;
using System.Globalization;
using System.Windows;
using System.Windows.Media;


namespace JFCGridControl
{
    public class CellImg : FrameworkElement, INotifyPropertyChanged
    {
        public static readonly DependencyProperty TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(CellImg), new PropertyMetadata("", CellImg.UpdateText));

        public static readonly DependencyProperty FontFamilyProperty = DependencyProperty.Register("FontFamily", typeof(FontFamily), typeof(CellImg), new FrameworkPropertyMetadata(SystemFonts.MessageFontFamily, FrameworkPropertyMetadataOptions.Inherits));

        public static readonly DependencyProperty FontSizeProperty = DependencyProperty.Register("FontSize", typeof(double), typeof(CellImg), new FrameworkPropertyMetadata(SystemFonts.MessageFontSize, FrameworkPropertyMetadataOptions.Inherits));

        public static readonly DependencyProperty ForegroundProperty = DependencyProperty.Register("Foreground", typeof(Brush), typeof(CellImg), new FrameworkPropertyMetadata(SystemColors.ControlTextBrush, FrameworkPropertyMetadataOptions.Inherits));

        public static readonly DependencyProperty TextAlignmentProperty = DependencyProperty.Register("TextAlignment", typeof(TextAlignment), typeof(CellImg), new UIPropertyMetadata(TextAlignment.Left, CellImg.UpdateText));

        public static readonly DependencyProperty HiliteBrushProperty = DependencyProperty.Register("HiliteColor", typeof(Brush), typeof(CellImg), new UIPropertyMetadata(new SolidColorBrush(Colors.Red), CellImg.UpdateText));

        public static readonly DependencyProperty HiliteTextProperty = DependencyProperty.Register("HiliteText", typeof(string), typeof(CellImg), new UIPropertyMetadata("", CellImg.UpdateText));

        public static readonly DependencyProperty ColumnProperty = DependencyProperty.Register("Column", typeof(JFCGridColumn), typeof(CellImg), new UIPropertyMetadata(null));

        private object data;

        private VisualCollection _children;

        private int childrenCount;

        private Size CellSize = new Size(0.0, 0.0);

        public string Text
        {
            get
            {
                return (string)base.GetValue(CellImg.TextProperty);
            }
            set
            {
                base.SetValue(CellImg.TextProperty, value);
            }
        }

        public FontFamily FontFamily
        {
            get
            {
                return (FontFamily)base.GetValue(CellImg.FontFamilyProperty);
            }
            set
            {
                base.SetValue(CellImg.FontFamilyProperty, value);
            }
        }

        public double FontSize
        {
            get
            {
                return (double)base.GetValue(CellImg.FontSizeProperty);
            }
            set
            {
                base.SetValue(CellImg.FontSizeProperty, value);
            }
        }

        public Brush Foreground
        {
            get
            {
                return (Brush)base.GetValue(CellImg.ForegroundProperty);
            }
            set
            {
                base.SetValue(CellImg.ForegroundProperty, value);
            }
        }

        public TextAlignment TextAlignment
        {
            get
            {
                return (TextAlignment)base.GetValue(CellImg.TextAlignmentProperty);
            }
            set
            {
                base.SetValue(CellImg.TextAlignmentProperty, value);
            }
        }

        public Brush HiliteBrush
        {
            get
            {
                return (Brush)base.GetValue(CellImg.HiliteBrushProperty);
            }
            set
            {
                base.SetValue(CellImg.HiliteBrushProperty, value);
            }
        }

        public string HiliteText
        {
            get
            {
                return (string)base.GetValue(CellImg.HiliteTextProperty);
            }
            set
            {
                base.SetValue(CellImg.HiliteTextProperty, value);
            }
        }

        public JFCGridColumn Column
        {
            get
            {
                return (JFCGridColumn)base.GetValue(CellImg.ColumnProperty);
            }
            set
            {
                base.SetValue(CellImg.ColumnProperty, value);
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
                object datum = this.data;
                this.data = value;
                this.OnPropertyChanged(new PropertyChangedEventArgs("Data"));
            }
        }

        protected override int VisualChildrenCount
        {
            get
            {
                return this.childrenCount;
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public CellImg()
        {
            base.ClipToBounds = true;
            base.Focusable = false;
            this._children = new VisualCollection(this);
            base.SizeChanged += this.CellImg_SizeChanged;
        }

        private void CellImg_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            CellImg cellImg = sender as CellImg;
            cellImg.childrenCount = 0;
            cellImg._children.Clear();
            cellImg._children.Add(cellImg.CreateDrawingVisualText());
            cellImg.childrenCount = 1;
            cellImg.InvalidateMeasure();
        }

        private static void UpdateText(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            CellImg cellImg = obj as CellImg;
            if (e.NewValue != e.OldValue)
            {
                if (e.Property == CellImg.HiliteBrushProperty && cellImg.HiliteText == "")
                {
                    return;
                }
                cellImg.CellSize = default(Size);
                cellImg.childrenCount = 0;
                cellImg._children.Clear();
                cellImg._children.Add(cellImg.CreateDrawingVisualText());
                cellImg.childrenCount = 1;
                cellImg.InvalidateMeasure();
            }
        }

        private DrawingVisual CreateDrawingVisualText()
        {
            string text = "";
            if (this.Text != null)
            {
                text = this.Text;
                if (this.Text == "")
                {
                    return null;
                }
                if (base.ActualWidth != 0.0 && base.ActualHeight != 0.0)
                {
                    DrawingVisual drawingVisual = new DrawingVisual();
                    DrawingContext drawingContext = drawingVisual.RenderOpen();
                    Point origin = new Point(0.0, 0.0);
                    FormattedText formattedText = new FormattedText(text, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(this.FontFamily.ToString()), this.FontSize, this.Foreground);
                    this.CellSize.Width = formattedText.Width;
                    this.CellSize.Height = formattedText.Height;
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
                    if (this.HiliteText != "")
                    {
                        int num = 0;
                        int length = this.HiliteText.Length;
                        num = text.ToLower().IndexOf(this.HiliteText.ToLower());
                        if (num >= 0)
                        {
                            formattedText.SetForegroundBrush(this.HiliteBrush, num, length);
                        }
                    }
                    drawingContext.DrawText(formattedText, origin);
                    drawingContext.Close();
                    return drawingVisual;
                }
                return null;
            }
            return null;
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            string text = "";
            if (this.Text != null)
            {
                text = this.Text;
                if (this.Text == "")
                {
                    return default(Size);
                }
                if (this.CellSize.Width == 0.0 && this.CellSize.Height == 0.0)
                {
                    new Point(0.0, 0.0);
                    FormattedText formattedText = new FormattedText(text, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(this.FontFamily.ToString()), this.FontSize, Brushes.Black);
                    this.CellSize.Width = formattedText.Width;
                    this.CellSize.Height = formattedText.Height;
                    return this.CellSize;
                }
                return this.CellSize;
            }
            return default(Size);
        }

        protected override Visual GetVisualChild(int index)
        {
            if (index >= 0 && index <= this.childrenCount)
            {
                return this._children[index];
            }
            throw new ArgumentOutOfRangeException();
        }

        internal void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (this.PropertyChanged != null)
            {
                this.PropertyChanged(this, e);
            }
        }

        public void ResetEvent()
        {
            this.PropertyChanged = null;
        }
    }
}
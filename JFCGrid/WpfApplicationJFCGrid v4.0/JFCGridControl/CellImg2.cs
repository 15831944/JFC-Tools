using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.ComponentModel;
using System.Windows.Media;
using System.Globalization;

namespace JFCGridControl
{
    public class CellImg2 : FrameworkElement, INotifyPropertyChanged
    {
        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Text.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TextProperty =
            DependencyProperty.Register("Text", typeof(string), typeof(CellImg2), new UIPropertyMetadata("", new PropertyChangedCallback(UpdateText)));
        //public static readonly DependencyProperty TextProperty =
        //    DependencyProperty.Register("Text", typeof(string), typeof(CellImg), new FrameworkPropertyMetadata("", FrameworkPropertyMetadataOptions.AffectsRender));

        public FontFamily FontFamily
        {
            get { return (FontFamily)GetValue(FontFamilyProperty); }
            set { SetValue(FontFamilyProperty, value); }
        }

        // Using a DependencyProperty as the backing store for FontFamily.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty FontFamilyProperty =
            DependencyProperty.Register("FontFamily", typeof(FontFamily), typeof(CellImg2), new FrameworkPropertyMetadata(SystemFonts.MessageFontFamily, FrameworkPropertyMetadataOptions.Inherits));

        public double FontSize
        {
            get { return (double)GetValue(FontSizeProperty); }
            set { SetValue(FontSizeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for FontSize.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty FontSizeProperty =
            DependencyProperty.Register("FontSize", typeof(double), typeof(CellImg2), new FrameworkPropertyMetadata(SystemFonts.MessageFontSize, FrameworkPropertyMetadataOptions.Inherits));

        public TextAlignment TextAlignment
        {
            get { return (TextAlignment)GetValue(TextAlignmentProperty); }
            set { SetValue(TextAlignmentProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TextAlignment.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TextAlignmentProperty =
            DependencyProperty.Register("TextAlignment", typeof(TextAlignment), typeof(CellImg2), new UIPropertyMetadata(TextAlignment.Left, new PropertyChangedCallback(UpdateText)));


        //public object Data = null;

        private object data = null;
        public object Data
        {
            get
            { return data; }
            set
            {
                data = value;
                OnPropertyChanged(new PropertyChangedEventArgs("Data"));
            }
        }

        private static void UpdateText(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            CellImg2 c = obj as CellImg2;

            if (e.NewValue != e.OldValue)
            {
                //c.UpdateLayout();


                // Create an instance of a DrawingVisual.
                DrawingVisual drawingVisual = new DrawingVisual();

                // Retrieve the DrawingContext from the DrawingVisual.
                DrawingContext drawingContext = drawingVisual.RenderOpen();

                c.OnRender(drawingContext);
            }
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            string testString = "";

            if (this.Text != null)
                testString = this.Text;
            else
                return;

            if (this.Text == "")
                return;


            System.Windows.Point position = new System.Windows.Point(0, 0);
            
            // Create the initial formatted text string.
            FormattedText FText = new FormattedText(testString, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(this.FontFamily.ToString()), this.FontSize, System.Windows.Media.Brushes.Black);

            if (TextAlignment == TextAlignment.Left)
            { }
            else if (TextAlignment == TextAlignment.Right)
                position.X = this.ActualWidth - FText.Width;
            else if (TextAlignment == TextAlignment.Center)
                position.X = (this.ActualWidth - FText.Width) / 2;
            else if (TextAlignment == TextAlignment.Justify)
            {
                //double coef = size.Width / this.ActualWidth;

                //for (int i = 0; i < advanceWidths.Count(); i++)
                //    advanceWidths[i] = advanceWidths[i] * coef;
            }

            //// Set a maximum width and height. If the text overflows these values, an ellipsis "..." appears.
            //formattedText.MaxTextWidth = 300;
            //formattedText.MaxTextHeight = 240;

            //// Use a larger font size beginning at the first (zero-based) character and continuing for 5 characters.
            //// The font size is calculated in terms of points -- not as device-independent pixels.
            //formattedText.SetFontSize(36 * (96.0 / 72.0), 0, 5);

            //// Use a Bold font weight beginning at the 6th character and continuing for 11 characters.
            //formattedText.SetFontWeight(FontWeights.Bold, 6, 11);

            //// Use a linear gradient brush beginning at the 6th character and continuing for 11 characters.
            //formattedText.SetForegroundBrush(
            //                        new LinearGradientBrush(
            //                        Colors.Orange,
            //                        Colors.Teal,
            //                        90.0),
            //                        6, 11);

            //// Use an Italic font style beginning at the 28th character and continuing for 28 characters.
            //formattedText.SetFontStyle(FontStyles.Italic, 28, 28);

            // Draw the formatted text string to the DrawingContext of the control.
            drawingContext.DrawText(FText, position);

        }


        //
        public event PropertyChangedEventHandler PropertyChanged;
        private void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }
    }
}

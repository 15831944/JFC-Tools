using System;
using System.ComponentModel;
using System.Globalization;
using System.Windows;
using System.Windows.Media;

namespace JFCGridControl
{
    public class CellImg : FrameworkElement, INotifyPropertyChanged
    {
        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Text.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TextProperty =
            DependencyProperty.Register("Text", typeof(string), typeof(CellImg), new PropertyMetadata("", new PropertyChangedCallback(UpdateText)));


        //public static readonly DependencyProperty TextProperty =
        //    DependencyProperty.Register("Text", typeof(string), typeof(CellImg), new UIPropertyMetadata("", new PropertyChangedCallback(UpdateText)));


        //private string text = "";
        //public string Text
        //{
        //    get { return text; }
        //    set
        //    {
        //        if (text != value)
        //        {
        //            text = value;
        //            OnPropertyChanged(new PropertyChangedEventArgs("Text"));
        //        }
        //    }        
        //}


        public FontFamily FontFamily
        {
            get { return (FontFamily)GetValue(FontFamilyProperty); }
            set { SetValue(FontFamilyProperty, value); }
        }

        // Using a DependencyProperty as the backing store for FontFamily.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty FontFamilyProperty =
            DependencyProperty.Register("FontFamily", typeof(FontFamily), typeof(CellImg), new FrameworkPropertyMetadata(SystemFonts.MessageFontFamily, FrameworkPropertyMetadataOptions.Inherits, new PropertyChangedCallback(UpdateText)));

        public double FontSize
        {
            get { return (double)GetValue(FontSizeProperty); }
            set { SetValue(FontSizeProperty, value); }
        }

        // Using a DependencyProperty as the backing store for FontSize.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty FontSizeProperty =
            DependencyProperty.Register("FontSize", typeof(double), typeof(CellImg), new FrameworkPropertyMetadata(SystemFonts.MessageFontSize, FrameworkPropertyMetadataOptions.Inherits, new PropertyChangedCallback(UpdateText)));

        public Brush Foreground
        {
            get { return (Brush)GetValue(ForegroundProperty); }
            set { SetValue(ForegroundProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Foreground.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ForegroundProperty =
            DependencyProperty.Register("Foreground", typeof(Brush), typeof(CellImg), new FrameworkPropertyMetadata(SystemColors.ControlTextBrush, FrameworkPropertyMetadataOptions.Inherits, new PropertyChangedCallback(UpdateText))); //new UIPropertyMetadata(SystemColors.ControlTextBrush));


        public TextAlignment TextAlignment
        {
            get { return (TextAlignment)GetValue(TextAlignmentProperty); }
            set { SetValue(TextAlignmentProperty, value); }
        }

        // Using a DependencyProperty as the backing store for TextAlignment.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty TextAlignmentProperty =
            DependencyProperty.Register("TextAlignment", typeof(TextAlignment), typeof(CellImg), new UIPropertyMetadata(TextAlignment.Left, new PropertyChangedCallback(UpdateText)));


        public Brush HiliteBrush
        {
            get { return (Brush)GetValue(HiliteBrushProperty); }
            set { SetValue(HiliteBrushProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HiliteColor.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HiliteBrushProperty =
            DependencyProperty.Register("HiliteColor", typeof(Brush), typeof(CellImg), new UIPropertyMetadata(new SolidColorBrush(Colors.Red), new PropertyChangedCallback(UpdateText)));


        public String HiliteText
        {
            get { return (String)GetValue(HiliteTextProperty); }
            set { SetValue(HiliteTextProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HiliteText.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HiliteTextProperty =
            DependencyProperty.Register("HiliteText", typeof(String), typeof(CellImg), new UIPropertyMetadata("", new PropertyChangedCallback(UpdateText)));




        public JFCGridColumn Column
        {
            get { return (JFCGridColumn)GetValue(ColumnProperty); }
            set { SetValue(ColumnProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Column.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ColumnProperty =
            DependencyProperty.Register("Column", typeof(JFCGridColumn), typeof(CellImg), new UIPropertyMetadata(null));



        //public object Data = null;

        private object data = null;
        public object Data
        {
            get
            {
                return data;
            }
            set
            {
                var oldValue = data;
                data = value;
                //OnPropertyChanged(new PropertyChangedExtendedEventArgs<Object>("Data", oldValue, value));
                OnPropertyChanged(new PropertyChangedEventArgs("Data"));
            }
        }

        //public object Data
        //{
        //    get { return (object)GetValue(DataProperty); }
        //    set { SetValue(DataProperty, value); }
        //}

        //// Using a DependencyProperty as the backing store for Data.  This enables animation, styling, binding, etc...
        //public static readonly DependencyProperty DataProperty =
        //    DependencyProperty.Register("Data", typeof(object), typeof(CellImg), new UIPropertyMetadata(null));



        // Create a collection of child visual objects.
        private VisualCollection _children;
        int childrenCount = 0;

        Size CellSize = new Size(0, 0);

        public CellImg()
        {
            this.ClipToBounds = true;
            this.Focusable = false;

            //this.SnapsToDevicePixels = true;

            _children = new VisualCollection(this);

            this.SizeChanged += new SizeChangedEventHandler(CellImg_SizeChanged);

            //this.UseLayoutRounding = true;

            //this.SetValue(RenderOptions.ClearTypeHintProperty, ClearTypeHint.Enabled);

        }

        void CellImg_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            CellImg cell = sender as CellImg;

            //if (cell.Text != "" && CellSize.Width == 0 && CellSize.Height == 0)
            //{
            cell.childrenCount = 0;
            cell._children.Clear();
            cell._children.Add(cell.CreateDrawingVisualText());
            cell.childrenCount = 1;
            //}

            cell.InvalidateMeasure();

            //DrawingVisual drawingVisual = new DrawingVisual();

            //DrawingContext drawingContext = drawingVisual.RenderOpen();

            //this.OnRender(drawingContext);
        }

        private static void UpdateText(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            CellImg c = obj as CellImg;

            if (e.NewValue != e.OldValue)
            {

                if (e.Property == HiliteBrushProperty)
                    if (c.HiliteText == "")
                        return;

                c.CellSize = new Size();

                c.childrenCount = 0;
                c._children.Clear();
                c._children.Add(c.CreateDrawingVisualText());
                c.childrenCount = 1;

                c.InvalidateMeasure();

                //DrawingVisual drawingVisual = new DrawingVisual();

                //DrawingContext drawingContext = drawingVisual.RenderOpen();

                //c.OnRender(drawingContext);
            }
        }


        private DrawingVisual CreateDrawingVisualText()
        {
            string testString = "";

            if (this.Text != null)
                testString = this.Text;
            else
                return null;

            if (this.Text == "")
                return null;

            if (this.ActualWidth == 0 || this.ActualHeight == 0)
                return null;

            // Create an instance of a DrawingVisual.
            DrawingVisual drawingVisual = new DrawingVisual();

            // Retrieve the DrawingContext from the DrawingVisual.
            DrawingContext drawingContext = drawingVisual.RenderOpen();

            System.Windows.Point position = new System.Windows.Point(0, 0);

            FormattedText FText = new FormattedText(testString, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(this.FontFamily.ToString()), this.FontSize, this.Foreground);

            //if (FText.Width < this.Width)
            //    size.Width = FText.Width;
            //else
            //    size.Width = this.Width;

            //if (FText.Height < this.Height)
            //    size.Height = FText.Height;
            //else
            //    size.Height = this.Height;

            // on récupère la taille du texte
            CellSize.Width = FText.Width;
            CellSize.Height = FText.Height;


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


            if (HiliteText != "")
            {
                int startIndex = 0;
                int count = HiliteText.Length;

                startIndex = testString.ToLower().IndexOf(HiliteText.ToLower());

                if (startIndex >= 0)
                    FText.SetForegroundBrush(HiliteBrush, startIndex, count);
            }

            // Draw a formatted text string into the DrawingContext.
            drawingContext.DrawText(FText, position);

            // Close the DrawingContext to persist changes to the DrawingVisual.
            drawingContext.Close();

            return drawingVisual;
        }


        //private DrawingVisual CreateDrawingVisualText()
        //{
        //    // Create an instance of a DrawingVisual.
        //    DrawingVisual drawingVisual = new DrawingVisual();

        //    // Retrieve the DrawingContext from the DrawingVisual.
        //    DrawingContext drawingContext = drawingVisual.RenderOpen();

        //    string testString = "";

        //    if (this.Text != null)
        //        testString = this.Text;
        //    else
        //        return null;

        //    if (this.Text == "")
        //        return null;

        //    GlyphTypeface glyphTypeface;

        //    Typeface typeFace = new Typeface(this.FontFamily.ToString());

        //    typeFace.TryGetGlyphTypeface(out glyphTypeface);

        //    System.Windows.Point position = new System.Windows.Point(0, this.FontSize);

        //    List<ushort> glyphIndices = new List<ushort>();
        //    List<double> advanceWidths = new List<double>();

        //    double longueur = 0;
        //    double fontSize = this.FontSize;
        //    double AdvanceWidth;
        //    ushort j;
        //    double tmp;

        //    foreach (Char c in testString.ToCharArray())
        //    {

        //        j = glyphTypeface.CharacterToGlyphMap[c];

        //        glyphIndices.Add(j);

        //        AdvanceWidth = glyphTypeface.AdvanceWidths[j];
        //        //double AdvanceHeight = glyphTypeface.AdvanceHeights[i];
        //        //double TopSideBearing = glyphTypeface.TopSideBearings[i];
        //        //double BottomSideBearing = glyphTypeface.BottomSideBearings[i];
        //        //double LeftSideBearing = glyphTypeface.LeftSideBearings[i];
        //        //double RightSideBearing = glyphTypeface.RightSideBearings[i];
        //        //double DistanceFromHorizontalBaselineToBlackBoxBottom = glyphTypeface.DistancesFromHorizontalBaselineToBlackBoxBottom[i];

        //        tmp = AdvanceWidth * fontSize;
        //        advanceWidths.Add(tmp);

        //        longueur += AdvanceWidth * fontSize;

        //    }

        //    if (TextAlignment == TextAlignment.Left)
        //    { }
        //    else if (TextAlignment == TextAlignment.Right)
        //        position.X = this.ActualWidth - longueur;
        //    else if (TextAlignment == TextAlignment.Center)
        //        position.X = (this.ActualWidth - longueur) / 2;
        //    else if (TextAlignment == TextAlignment.Justify)
        //    {
        //        double coef = longueur / this.ActualWidth;

        //        for (int i = 0; i < advanceWidths.Count(); i++)
        //            advanceWidths[i] = advanceWidths[i] * coef;
        //    }

        //    GlyphRun theGlyphRun = new GlyphRun(glyphTypeface, 0, false, fontSize,
        //        glyphIndices,
        //        position, advanceWidths, null, null, null, null, null, null);

        //    drawingContext.DrawGlyphRun(Brushes.Black, theGlyphRun);



        //    // Close the DrawingContext to persist changes to the DrawingVisual.
        //    drawingContext.Close();

        //    //DrawingVisual d = new DrawingVisual();

        //    //d.Children.Add(Visual);

        //    return drawingVisual;
        //}

        //protected override void OnRender(DrawingContext drawingContext)
        //{
        //    string testString = "";

        //    if (this.Text != null)
        //        testString = this.Text;
        //    else
        //        return;

        //    if (this.Text == "")
        //        return;

        //    GlyphTypeface glyphTypeface;

        //    Typeface typeFace = new Typeface(this.FontFamily.ToString());

        //    typeFace.TryGetGlyphTypeface(out glyphTypeface);

        //    System.Windows.Point position = new System.Windows.Point(0, this.FontSize);

        //    List<ushort> glyphIndices = new List<ushort>();
        //    List<double> advanceWidths = new List<double>();

        //    double longueur = 0;

        //    foreach (Char c in testString.ToCharArray())
        //    {

        //        ushort i = glyphTypeface.CharacterToGlyphMap[c];

        //        glyphIndices.Add(i);

        //        double AdvanceWidth = glyphTypeface.AdvanceWidths[i];
        //        double AdvanceHeight = glyphTypeface.AdvanceHeights[i];
        //        double TopSideBearing = glyphTypeface.TopSideBearings[i];
        //        double BottomSideBearing = glyphTypeface.BottomSideBearings[i];
        //        double LeftSideBearing = glyphTypeface.LeftSideBearings[i];
        //        double RightSideBearing = glyphTypeface.RightSideBearings[i];
        //        double DistanceFromHorizontalBaselineToBlackBoxBottom = glyphTypeface.DistancesFromHorizontalBaselineToBlackBoxBottom[i];


        //        advanceWidths.Add(AdvanceWidth * this.FontSize);

        //        longueur += AdvanceWidth * this.FontSize;

        //    }

        //    if (TextAlignment == TextAlignment.Left)
        //    { }
        //    else if (TextAlignment == TextAlignment.Right)
        //        position.X = this.ActualWidth - longueur;
        //    else if (TextAlignment == TextAlignment.Center)
        //        position.X = (this.ActualWidth - longueur) / 2;
        //    else if (TextAlignment == TextAlignment.Justify)
        //    {
        //        double coef = longueur / this.ActualWidth;

        //        for (int i = 0; i < advanceWidths.Count(); i++)
        //            advanceWidths[i] = advanceWidths[i] * coef;
        //    }

        //    GlyphRun theGlyphRun = new GlyphRun(glyphTypeface, 0, false, this.FontSize,
        //        glyphIndices,
        //        position, advanceWidths, null, null, null, null, null, null);

        //    drawingContext.DrawGlyphRun(Brushes.Black, theGlyphRun);



        //    // Close the DrawingContext to persist changes to the DrawingVisual.
        //    //drawingContext.Close();

        //    //base.OnRender(drawingContext);
        //}

        //protected override Size ArrangeOverride(Size finalSize)
        //{
        //    string testString = "";

        //    if (this.Text != null)
        //        testString = this.Text;

        //    FormattedText FText = new FormattedText(testString, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(this.FontFamily.ToString()), this.FontSize, System.Windows.Media.Brushes.Black);

        //    Size fsize = new Size();

        //    if (FText.Width < finalSize.Width)
        //        fsize.Width = FText.Width;
        //    else
        //        fsize.Width = finalSize.Width;

        //    if (FText.Height < finalSize.Height)
        //        fsize.Height = FText.Height;
        //    else
        //        fsize.Height = finalSize.Height;

        //    return fsize;
        //}

        protected override Size MeasureOverride(Size availableSize)
        {
            //string testString = "";

            //if (this.Text != null)
            //    testString = this.Text;

            //Point Pt = new System.Windows.Point(0, 0);

            ////FormattedText FText = new FormattedText(testString,
            ////      Thread.CurrentThread.CurrentCulture,
            ////      FlowDirection.LeftToRight,
            ////      new Typeface(this.FontFamily.ToString()),
            ////      this.FontSize, System.Windows.Media.Brushes.Black);

            //FormattedText FText = new FormattedText(testString, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(this.FontFamily.ToString()), this.FontSize, System.Windows.Media.Brushes.Black);

            //// il ne faut pas mettre l'alignement car si non on a plus la taille....
            ////FText.TextAlignment = this.TextAlignment;

            //Size size = new Size();

            ////if (FText.Width < availableSize.Width)
            //    size.Width = FText.Width;
            ////else
            ////    size.Width = availableSize.Width;

            ////if (FText.Height < availableSize.Height)
            //    size.Height = FText.Height;
            ////else
            ////    size.Height = availableSize.Height;

            //return size;


            string testString = "";

            if (this.Text != null)
                testString = this.Text;
            else
                return new Size();

            if (this.Text == "")
                return new Size();

            if (CellSize.Width != 0 || CellSize.Height != 0)
                return CellSize;

            System.Windows.Point position = new System.Windows.Point(0, 0);

            FormattedText FText = new FormattedText(testString, CultureInfo.GetCultureInfo("en-us"), FlowDirection.LeftToRight, new Typeface(this.FontFamily.ToString()), this.FontSize, System.Windows.Media.Brushes.Black);

            // on récupère la taille du texte
            CellSize.Width = FText.Width;
            CellSize.Height = FText.Height;

            return CellSize;
        }

        // Provide a required override for the VisualChildrenCount property.
        protected override int VisualChildrenCount
        {
            //get { return 0; }
            //get { return _children.Count; }
            get { return childrenCount; }
        }

        // Provide a required override for the GetVisualChild method.
        protected override Visual GetVisualChild(int index)
        {
            //if (index < 0 || index > _children.Count)
            if (index < 0 || index > childrenCount)
            {
                throw new ArgumentOutOfRangeException();
            }

            return _children[index];
        }

        ////
        //public event PropertyChangedExtendedEventHandler<Object> PropertyChanged;
        //internal void OnPropertyChanged(PropertyChangedExtendedEventArgs<Object> e)
        //{
        //    if (PropertyChanged != null)
        //        PropertyChanged(this, e);
        //}

        public event PropertyChangedEventHandler PropertyChanged;
        internal void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }

        public void ResetEvent()
        {
            PropertyChanged = null;
        }


    }
}


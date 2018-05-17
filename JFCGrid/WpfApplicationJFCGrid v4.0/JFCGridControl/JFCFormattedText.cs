using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.TextFormatting;
using System.Windows;
using System.ComponentModel;
using System.Collections;
using System.Globalization;
using System.Windows.Threading;
using System.Windows.Media.Animation;
using System.Runtime.InteropServices;


namespace JFCGridControl
{
    //public class JFCFormattedText //: FormattedText
    //{

    //    private GenericTextParagraphProperties _defaultParaProps;
    //    //private SpanVector _formatRuns;
    //    //private SpanPosition _latestPosition;
    //    private int _maxLineCount;
    //    private double _maxTextHeight;
    //    private double _maxTextWidth;
    //    private double[] _maxTextWidths;
    //    private CachedMetrics _metrics;
    //    private double _minWidth;
    //    private string _text;
    //    private TextSourceImplementation _textSourceImpl;
    //    private TextTrimming _trimming;
    //    private const double MaxFontEmSize = 35791.394066666668;
    //    private const double RealInfiniteWidth = 3579139.4066666667;



    //    public JFCFormattedText(string textToFormat, CultureInfo culture, FlowDirection flowDirection, Typeface typeface, double emSize, Brush foreground)
    //        : this(textToFormat, culture, flowDirection, typeface, emSize, foreground, null)
    //    {
    //    }

    //    public JFCFormattedText(string textToFormat, CultureInfo culture, FlowDirection flowDirection, Typeface typeface, double emSize, Brush foreground, NumberSubstitution numberSubstitution)
    //    {
    //        //this._formatRuns = new SpanVector(null);
    //        //this._latestPosition = new SpanPosition();
    //        //this._maxTextHeight = double.MaxValue;
    //        //this._maxLineCount = 0x7fffffff;
    //        //this._trimming = TextTrimming.WordEllipsis;
    //        if (textToFormat == null)
    //        {
    //            throw new ArgumentNullException("textToFormat");
    //        }
    //        if (typeface == null)
    //        {
    //            throw new ArgumentNullException("typeface");
    //        }
    //        //ValidateCulture(culture);
    //        //ValidateFlowDirection(flowDirection, "flowDirection");
    //        ValidateFontSize(emSize);
    //        this._text = textToFormat;
    //        GenericTextRunProperties element = new GenericTextRunProperties(typeface, emSize, 12.0, null, foreground, null, BaselineAlignment.Baseline, culture, numberSubstitution);
    //        //this._latestPosition = this._formatRuns.SetValue(0, this._text.Length, element, this._latestPosition);
    //        this._defaultParaProps = new GenericTextParagraphProperties(flowDirection, TextAlignment.Left, false, false, element, TextWrapping.WrapWithOverflow, 0.0, 0.0);
    //        this.InvalidateMetrics();
    //    }


    //    internal void Draw(DrawingContext dc, Point origin)
    //    {
    //        Point point = origin;
    //        if ((this._metrics != null) && !double.IsNaN(this._metrics.Extent))
    //        {
    //            using (LineEnumerator enumerator = this.GetEnumerator())
    //            {
    //                while (enumerator.MoveNext())
    //                {
    //                    using (TextLine line = enumerator.Current)
    //                    {
    //                        line.Draw(dc, point, InvertAxes.None);
    //                        this.AdvanceLineOrigin(ref point, line);
    //                        continue;
    //                    }
    //                }
    //                return;
    //            }
    //        }
    //        this._metrics = this.DrawAndCalculateMetrics(dc, origin, true);
    //    }

    //    private LineEnumerator GetEnumerator()
    //    {
    //        return new LineEnumerator(this);
    //    }

    //    private static void ValidateFontSize(double emSize)
    //    {
    //        //if (emSize <= 0.0)
    //        //{
    //        //    throw new ArgumentOutOfRangeException("emSize", SR.Get("ParameterMustBeGreaterThanZero"));
    //        //}
    //        //if (emSize > 35791.394066666668)
    //        //{
    //        //    throw new ArgumentOutOfRangeException("emSize", SR.Get("ParameterCannotBeGreaterThan", new object[] { 35791.394066666668 }));
    //        //}
    //        //if (DoubleUtil.IsNaN(emSize))
    //        //{
    //        //    throw new ArgumentOutOfRangeException("emSize", SR.Get("ParameterValueCannotBeNaN"));
    //        //}
    //    }

    //    private void InvalidateMetrics()
    //    {
    //        this._metrics = null;
    //        this._minWidth = double.MinValue;
    //    }

    //    private void AdvanceLineOrigin(ref Point lineOrigin, TextLine currentLine)
    //    {
    //        double height = currentLine.Height;
    //        switch (this._defaultParaProps.FlowDirection)
    //        {
    //            case FlowDirection.LeftToRight:
    //            case FlowDirection.RightToLeft:
    //                lineOrigin.Y += height;
    //                return;
    //        }
    //    }

    //    private CachedMetrics DrawAndCalculateMetrics(DrawingContext dc, Point drawingOffset, bool getBlackBoxMetrics)
    //    {
    //        CachedMetrics metrics = new CachedMetrics();
    //        if (this._text.Length != 0)
    //        {
    //            using (LineEnumerator enumerator = this.GetEnumerator())
    //            {
    //                double num2;
    //                double num4;
    //                bool flag = true;
    //                double num3 = num4 = double.MaxValue;
    //                double num = num2 = double.MinValue;
    //                Point lineOrigin = new Point(0.0, 0.0);
    //                while (enumerator.MoveNext())
    //                {
    //                    using (TextLine line = enumerator.Current)
    //                    {
    //                        if (dc != null)
    //                        {
    //                            line.Draw(dc, new Point(lineOrigin.X + drawingOffset.X, lineOrigin.Y + drawingOffset.Y), InvertAxes.None);
    //                        }
    //                        if (getBlackBoxMetrics)
    //                        {
    //                            double num8 = (lineOrigin.X + line.Start) + line.OverhangLeading;
    //                            double num7 = ((lineOrigin.X + line.Start) + line.Width) - line.OverhangTrailing;
    //                            double num5 = (lineOrigin.Y + line.Height) + line.OverhangAfter;
    //                            double num6 = num5 - line.Extent;
    //                            num3 = Math.Min(num3, num8);
    //                            num = Math.Max(num, num7);
    //                            num2 = Math.Max(num2, num5);
    //                            num4 = Math.Min(num4, num6);
    //                            metrics.OverhangAfter = line.OverhangAfter;
    //                        }
    //                        metrics.Height += line.Height;
    //                        metrics.Width = Math.Max(metrics.Width, line.Width + line.Start);
    //                        metrics.WidthIncludingTrailingWhitespace = Math.Max(metrics.WidthIncludingTrailingWhitespace, line.WidthIncludingTrailingWhitespace + line.Start);
    //                        if (flag)
    //                        {
    //                            metrics.Baseline = line.Baseline;
    //                            flag = false;
    //                        }
    //                        this.AdvanceLineOrigin(ref lineOrigin, line);
    //                        continue;
    //                    }
    //                }
    //                if (getBlackBoxMetrics)
    //                {
    //                    metrics.Extent = num2 - num4;
    //                    metrics.OverhangLeading = num3;
    //                    metrics.OverhangTrailing = metrics.Width - num;
    //                    return metrics;
    //                }
    //                metrics.Extent = double.NaN;
    //            }
    //        }
    //        return metrics;
    //    }



    //    [StructLayout(LayoutKind.Sequential)]
    //    private struct LineEnumerator : IEnumerator, IDisposable
    //    {
    //        private int _textStorePosition;
    //        private int _lineCount;
    //        private double _totalHeight;
    //        private TextLine _currentLine;
    //        private TextLine _nextLine;
    //        private TextFormatter _formatter;
    //        private JFCFormattedText _that;
    //        private double _previousHeight;
    //        private int _previousLength;
    //        private TextLineBreak _previousLineBreak;

    //        internal LineEnumerator(JFCFormattedText text)
    //        {
    //            this._previousHeight = 0.0;
    //            this._previousLength = 0;
    //            this._previousLineBreak = null;
    //            this._textStorePosition = 0;
    //            this._lineCount = 0;
    //            this._totalHeight = 0.0;
    //            this._currentLine = null;
    //            this._nextLine = null;
    //            this._formatter = JFCTextFormatter.FromCurrentDispatcher();
    //            this._that = text;
    //            if (this._that._textSourceImpl == null)
    //            {
    //                this._that._textSourceImpl = new JFCFormattedText.TextSourceImplementation(this._that);
    //            }
    //        }

    //        public void Dispose()
    //        {
    //            if (this._currentLine != null)
    //            {
    //                this._currentLine.Dispose();
    //                this._currentLine = null;
    //            }
    //            if (this._nextLine != null)
    //            {
    //                this._nextLine.Dispose();
    //                this._nextLine = null;
    //            }
    //        }

    //        internal int Position
    //        {
    //            get
    //            {
    //                return this._textStorePosition;
    //            }
    //        }
    //        internal int Length
    //        {
    //            get
    //            {
    //                return this._previousLength;
    //            }
    //        }
    //        public TextLine Current
    //        {
    //            get
    //            {
    //                return this._currentLine;
    //            }
    //        }
    //        object IEnumerator.Current
    //        {
    //            get
    //            {
    //                return this.Current;
    //            }
    //        }
    //        internal double CurrentParagraphWidth
    //        {
    //            get
    //            {
    //                return this.MaxLineLength(this._lineCount);
    //            }
    //        }
    //        private double MaxLineLength(int line)
    //        {
    //            if (this._that._maxTextWidths == null)
    //            {
    //                return this._that._maxTextWidth;
    //            }
    //            return this._that._maxTextWidths[Math.Min(line, this._that._maxTextWidths.Length - 1)];
    //        }

    //        public bool MoveNext()
    //        {
    //            if (this._currentLine == null)
    //            {
    //                if (this._that._text.Length == 0)
    //                {
    //                    return false;
    //                }
    //                this._currentLine = this.FormatLine(this._that._textSourceImpl, this._textStorePosition, this.MaxLineLength(this._lineCount), this._that._defaultParaProps, null);
    //                if ((this._totalHeight + this._currentLine.Height) > this._that._maxTextHeight)
    //                {
    //                    this._currentLine.Dispose();
    //                    this._currentLine = null;
    //                    return false;
    //                }
    //            }
    //            else
    //            {
    //                if (this._nextLine == null)
    //                {
    //                    return false;
    //                }
    //                this._totalHeight += this._previousHeight;
    //                this._textStorePosition += this._previousLength;
    //                this._lineCount++;
    //                this._currentLine = this._nextLine;
    //                this._nextLine = null;
    //            }
    //            TextLineBreak textLineBreak = this._currentLine.GetTextLineBreak();
    //            if ((this._textStorePosition + this._currentLine.Length) < this._that._text.Length)
    //            {
    //                bool flag = false;
    //                //bool flag;
    //                //if ((this._lineCount + 1) >= this._that._maxLineCount)
    //                //{
    //                //    flag = false;
    //                //}
    //                //else
    //                //{
    //                //    this._nextLine = this.FormatLine(this._that._textSourceImpl, this._textStorePosition + this._currentLine.Length, this.MaxLineLength(this._lineCount + 1), this._that._defaultParaProps, textLineBreak);
    //                //    flag = ((this._totalHeight + this._currentLine.Height) + this._nextLine.Height) <= this._that._maxTextHeight;
    //                //}
    //                if (!flag)
    //                {
    //                    if (this._nextLine != null)
    //                    {
    //                        this._nextLine.Dispose();
    //                        this._nextLine = null;
    //                    }
    //                    //if ((this._that._trimming != TextTrimming.None) && !this._currentLine.HasCollapsed)
    //                    //{
    //                    //    TextWrapping textWrapping = this._that._defaultParaProps.TextWrapping;
    //                    //    this._that._defaultParaProps.SetTextWrapping(TextWrapping.NoWrap);
    //                    //    if (textLineBreak != null)
    //                    //    {
    //                    //        textLineBreak.Dispose();
    //                    //    }
    //                    //    this._currentLine.Dispose();
    //                    //    this._currentLine = this.FormatLine(this._that._textSourceImpl, this._textStorePosition, this.MaxLineLength(this._lineCount), this._that._defaultParaProps, this._previousLineBreak);
    //                    //    textLineBreak = this._currentLine.GetTextLineBreak();
    //                    //    this._that._defaultParaProps.SetTextWrapping(textWrapping);
    //                    //}
    //                }
    //            }
    //            this._previousHeight = this._currentLine.Height;
    //            this._previousLength = this._currentLine.Length;
    //            if (this._previousLineBreak != null)
    //            {
    //                this._previousLineBreak.Dispose();
    //            }
    //            this._previousLineBreak = textLineBreak;
    //            return true;
    //        }

    //        private TextLine FormatLine(TextSource textSource, int textSourcePosition, double maxLineLength, TextParagraphProperties paraProps, TextLineBreak lineBreak)
    //        {
    //            TextLine line = this._formatter.FormatLine(textSource, textSourcePosition, maxLineLength, paraProps, lineBreak);
    //            //if (((this._that._trimming != TextTrimming.None) && line.HasOverflowed) && (line.Length > 0))
    //            //{
    //            //    TextCollapsingProperties properties;
    //            //    SpanRider rider = new SpanRider(this._that._formatRuns, this._that._latestPosition, Math.Min((int)((textSourcePosition + line.Length) - 1), (int)(this._that._text.Length - 1)));
    //            //    GenericTextRunProperties currentElement = rider.CurrentElement as GenericTextRunProperties;
    //            //    if (this._that._trimming == TextTrimming.CharacterEllipsis)
    //            //    {
    //            //        properties = new TextTrailingCharacterEllipsis(maxLineLength, currentElement);
    //            //    }
    //            //    else
    //            //    {
    //            //        properties = new TextTrailingWordEllipsis(maxLineLength, currentElement);
    //            //    }
    //            //    TextLine line2 = line.Collapse(new TextCollapsingProperties[] { properties });
    //            //    if (line2 != line)
    //            //    {
    //            //        line.Dispose();
    //            //        line = line2;
    //            //    }
    //            //}
    //            return line;
    //        }

    //        public void Reset()
    //        {
    //            this._textStorePosition = 0;
    //            this._lineCount = 0;
    //            this._totalHeight = 0.0;
    //            this._currentLine = null;
    //            this._nextLine = null;
    //        }
    //    }


    //    private class CachedMetrics
    //    {
    //        // Fields
    //        public double Baseline;
    //        public double Extent;
    //        public double Height;
    //        public double OverhangAfter;
    //        public double OverhangLeading;
    //        public double OverhangTrailing;
    //        public double Width;
    //        public double WidthIncludingTrailingWhitespace;
    //    }


    //    private class TextSourceImplementation : TextSource
    //    {
    //        // Fields
    //        private JFCFormattedText _that;

    //        // Methods
    //        public TextSourceImplementation(JFCFormattedText text)
    //        {
    //            this._that = text;
    //        }

    //        public override TextSpan<CultureSpecificCharacterBufferRange> GetPrecedingText(int textSourceCharacterIndexLimit)
    //        {
    //            //CharacterBufferRange empty = CharacterBufferRange.Empty;
    //            //CultureInfo culture = null;
    //            //if (textSourceCharacterIndexLimit > 0)
    //            //{
    //            //    SpanRider rider = new SpanRider(this._that._formatRuns, this._that._latestPosition, textSourceCharacterIndexLimit - 1);
    //            //    empty = new CharacterBufferRange(new CharacterBufferReference(this._that._text, rider.CurrentSpanStart), textSourceCharacterIndexLimit - rider.CurrentSpanStart);
    //            //    culture = ((TextRunProperties)rider.CurrentElement).CultureInfo;
    //            //}
    //            //return new TextSpan<CultureSpecificCharacterBufferRange>(empty.Length, new CultureSpecificCharacterBufferRange(culture, empty));

    //            return null;
    //        }

    //        public override int GetTextEffectCharacterIndexFromTextSourceCharacterIndex(int textSourceCharacterIndex)
    //        {
    //            throw new NotSupportedException();
    //        }

    //        public override TextRun GetTextRun(int textSourceCharacterIndex)
    //        {
    //            if (textSourceCharacterIndex >= this._that._text.Length)
    //            {
    //                return new TextEndOfParagraph(1);
    //            }
    //            SpanRider rider = new SpanRider(this._that._formatRuns, this._that._latestPosition, textSourceCharacterIndex);
    //            return new TextCharacters(this._that._text, textSourceCharacterIndex, rider.Length, rider.CurrentElement as GenericTextRunProperties);
    //        }
    //    }

    //    //[StructLayout(LayoutKind.Sequential)]
    //    //internal struct SpanRider
    //    //{
    //    //    private SpanVector _spans;
    //    //    private SpanPosition _spanPosition;
    //    //    private int _cp;
    //    //    private int _cch;
    //    //    public SpanRider(SpanVector spans)
    //    //        : this(spans, new SpanPosition(), 0)
    //    //    {
    //    //    }

    //    //    public SpanRider(SpanVector spans, SpanPosition latestPosition)
    //    //        : this(spans, latestPosition, latestPosition.CP)
    //    //    {
    //    //    }

    //    //    public SpanRider(SpanVector spans, SpanPosition latestPosition, int cp)
    //    //    {
    //    //        this._spans = spans;
    //    //        this._spanPosition = new SpanPosition();
    //    //        this._cp = 0;
    //    //        this._cch = 0;
    //    //        this.At(latestPosition, cp);
    //    //    }

    //    //    public bool At(int cp)
    //    //    {
    //    //        return this.At(this._spanPosition, cp);
    //    //    }

    //    //    public bool At(SpanPosition latestPosition, int cp)
    //    //    {
    //    //        bool flag = this._spans.FindSpan(cp, latestPosition, out this._spanPosition);
    //    //        if (flag)
    //    //        {
    //    //            this._cch = this._spans[this._spanPosition.Index].length - (cp - this._spanPosition.CP);
    //    //            this._cp = cp;
    //    //            return flag;
    //    //        }
    //    //        this._cch = 0x7fffffff;
    //    //        this._cp = this._spanPosition.CP;
    //    //        return flag;
    //    //    }

    //    //    public int CurrentSpanStart
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._spanPosition.CP;
    //    //        }
    //    //    }
    //    //    public int Length
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._cch;
    //    //        }
    //    //    }
    //    //    public int CurrentPosition
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._cp;
    //    //        }
    //    //    }
    //    //    public object CurrentElement
    //    //    {
    //    //        get
    //    //        {
    //    //            if (this._spanPosition.Index < this._spans.Count)
    //    //            {
    //    //                return this._spans[this._spanPosition.Index].element;
    //    //            }
    //    //            return this._spans.Default;
    //    //        }
    //    //    }
    //    //    public int CurrentSpanIndex
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._spanPosition.Index;
    //    //        }
    //    //    }
    //    //    public SpanPosition SpanPosition
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._spanPosition;
    //    //        }
    //    //    }
    //    //}

    //    //[StructLayout(LayoutKind.Sequential)]
    //    //internal struct SpanPosition
    //    //{
    //    //    private int _spanIndex;
    //    //    private int _spanCP;
    //    //    internal SpanPosition(int spanIndex, int spanCP)
    //    //    {
    //    //        this._spanIndex = spanIndex;
    //    //        this._spanCP = spanCP;
    //    //    }

    //    //    internal int Index
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._spanIndex;
    //    //        }
    //    //    }
    //    //    internal int CP
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._spanCP;
    //    //        }
    //    //    }
    //    //}

    //    //internal class SpanVector : IEnumerable
    //    //{
    //    //    // Fields
    //    //    private object _defaultObject;
    //    //    //private static Equals _equals = new Equals(object.Equals);
    //    //    //private static Equals _referenceEquals = new Equals(object.ReferenceEquals);
    //    //    //private FrugalStructList<Span> _spans;

    //    //    // Methods
    //    //    public SpanVector(object defaultObject)
    //    //        : this(defaultObject, new FrugalStructList<Span>())
    //    //    {
    //    //    }

    //    //    private SpanVector(object defaultObject, FrugalStructList<Span> spans)
    //    //    {
    //    //        this._defaultObject = defaultObject;
    //    //        this._spans = spans;
    //    //    }

    //    //    private void Add(Span span)
    //    //    {
    //    //        this._spans.Add(span);
    //    //    }

    //    //    internal virtual void Delete(int index, int count, ref SpanPosition latestPosition)
    //    //    {
    //    //        this.DeleteInternal(index, count);
    //    //        if (index <= latestPosition.Index)
    //    //        {
    //    //            latestPosition = new SpanPosition();
    //    //        }
    //    //    }

    //    //    private void DeleteInternal(int index, int count)
    //    //    {
    //    //        for (int i = (index + count) - 1; i >= index; i--)
    //    //        {
    //    //            this._spans.RemoveAt(i);
    //    //        }
    //    //    }

    //    //    internal bool FindSpan(int cp, SpanPosition latestPosition, out SpanPosition spanPosition)
    //    //    {
    //    //        int index;
    //    //        int cP;
    //    //        int count = this._spans.Count;
    //    //        if (cp == 0)
    //    //        {
    //    //            index = 0;
    //    //            cP = 0;
    //    //        }
    //    //        else if ((cp >= latestPosition.CP) || ((cp * 2) < latestPosition.CP))
    //    //        {
    //    //            if (cp >= latestPosition.CP)
    //    //            {
    //    //                index = latestPosition.Index;
    //    //                cP = latestPosition.CP;
    //    //            }
    //    //            else
    //    //            {
    //    //                index = 0;
    //    //                cP = 0;
    //    //            }
    //    //            while (index < count)
    //    //            {
    //    //                int length = this._spans[index].length;
    //    //                if (cp < (cP + length))
    //    //                {
    //    //                    break;
    //    //                }
    //    //                cP += length;
    //    //                index++;
    //    //            }
    //    //        }
    //    //        else
    //    //        {
    //    //            index = latestPosition.Index;
    //    //            cP = latestPosition.CP;
    //    //            while (cP > cp)
    //    //            {
    //    //                cP -= this._spans[--index].length;
    //    //            }
    //    //        }
    //    //        spanPosition = new SpanPosition(index, cP);
    //    //        return (index != count);
    //    //    }

    //    //    public IEnumerator GetEnumerator()
    //    //    {
    //    //        return new SpanEnumerator(this);
    //    //    }

    //    //    private void Insert(int index, int count)
    //    //    {
    //    //        for (int i = 0; i < count; i++)
    //    //        {
    //    //            this._spans.Insert(index, new Span(null, 0));
    //    //        }
    //    //    }

    //    //    private bool Resize(int targetCount)
    //    //    {
    //    //        if (targetCount > this.Count)
    //    //        {
    //    //            for (int i = 0; i < (targetCount - this.Count); i++)
    //    //            {
    //    //                this._spans.Add(new Span(null, 0));
    //    //            }
    //    //        }
    //    //        else if (targetCount < this.Count)
    //    //        {
    //    //            this.DeleteInternal(targetCount, this.Count - targetCount);
    //    //        }
    //    //        return true;
    //    //    }

    //    //    //private SpanPosition Set(int first, int length, object element, Equals equals, SpanPosition spanPosition)
    //    //    //{
    //    //    //    bool flag = this.FindSpan(first, spanPosition, out spanPosition);
    //    //    //    int index = spanPosition.Index;
    //    //    //    int cP = spanPosition.CP;
    //    //    //    if (!flag)
    //    //    //    {
    //    //    //        if (cP < first)
    //    //    //        {
    //    //    //            this.Add(new Span(this._defaultObject, first - cP));
    //    //    //        }
    //    //    //        if ((this.Count > 0) && equals(this._spans[this.Count - 1].element, element))
    //    //    //        {
    //    //    //            Span local1 = this._spans[this.Count - 1];
    //    //    //            local1.length += length;
    //    //    //            if (index == this.Count)
    //    //    //            {
    //    //    //                cP += length;
    //    //    //            }
    //    //    //        }
    //    //    //        else
    //    //    //        {
    //    //    //            this.Add(new Span(element, length));
    //    //    //        }
    //    //    //    }
    //    //    //    else
    //    //    //    {
    //    //    //        int num3 = index;
    //    //    //        int num4 = cP;
    //    //    //        while ((num3 < this.Count) && ((num4 + this._spans[num3].length) <= (first + length)))
    //    //    //        {
    //    //    //            num4 += this._spans[num3].length;
    //    //    //            num3++;
    //    //    //        }
    //    //    //        if (first == cP)
    //    //    //        {
    //    //    //            if ((index > 0) && equals(this._spans[index - 1].element, element))
    //    //    //            {
    //    //    //                index--;
    //    //    //                cP -= this._spans[index].length;
    //    //    //                first = cP;
    //    //    //                length += this._spans[index].length;
    //    //    //            }
    //    //    //        }
    //    //    //        else if (equals(this._spans[index].element, element))
    //    //    //        {
    //    //    //            length = (first + length) - cP;
    //    //    //            first = cP;
    //    //    //        }
    //    //    //        if ((num3 < this.Count) && equals(this._spans[num3].element, element))
    //    //    //        {
    //    //    //            length = (num4 + this._spans[num3].length) - first;
    //    //    //            num4 += this._spans[num3].length;
    //    //    //            num3++;
    //    //    //        }
    //    //    //        if (num3 >= this.Count)
    //    //    //        {
    //    //    //            if (cP < first)
    //    //    //            {
    //    //    //                if ((this.Count != (index + 2)) && !this.Resize(index + 2))
    //    //    //                {
    //    //    //                    throw new OutOfMemoryException();
    //    //    //                }
    //    //    //                this._spans[index].length = first - cP;
    //    //    //                this._spans[index + 1] = new Span(element, length);
    //    //    //            }
    //    //    //            else
    //    //    //            {
    //    //    //                if ((this.Count != (index + 1)) && !this.Resize(index + 1))
    //    //    //                {
    //    //    //                    throw new OutOfMemoryException();
    //    //    //                }
    //    //    //                this._spans[index] = new Span(element, length);
    //    //    //            }
    //    //    //        }
    //    //    //        else
    //    //    //        {
    //    //    //            object obj2 = null;
    //    //    //            int num7 = 0;
    //    //    //            if ((first + length) > num4)
    //    //    //            {
    //    //    //                obj2 = this._spans[num3].element;
    //    //    //                num7 = (num4 + this._spans[num3].length) - (first + length);
    //    //    //            }
    //    //    //            int count = (1 + ((first > cP) ? 1 : 0)) - (num3 - index);
    //    //    //            if (count < 0)
    //    //    //            {
    //    //    //                this.DeleteInternal(index + 1, -count);
    //    //    //            }
    //    //    //            else if (count > 0)
    //    //    //            {
    //    //    //                this.Insert(index + 1, count);
    //    //    //                for (int i = 0; i < count; i++)
    //    //    //                {
    //    //    //                    this._spans[(index + 1) + i] = new Span(null, 0);
    //    //    //                }
    //    //    //            }
    //    //    //            if (cP < first)
    //    //    //            {
    //    //    //                this._spans[index].length = first - cP;
    //    //    //                index++;
    //    //    //                cP = first;
    //    //    //            }
    //    //    //            this._spans[index] = new Span(element, length);
    //    //    //            index++;
    //    //    //            cP += length;
    //    //    //            if (num4 < (first + length))
    //    //    //            {
    //    //    //                this._spans[index] = new Span(obj2, num7);
    //    //    //            }
    //    //    //        }
    //    //    //    }
    //    //    //    return new SpanPosition(index, cP);
    //    //    //}

    //    //    //public void SetReference(int first, int length, object element)
    //    //    //{
    //    //    //    this.Set(first, length, element, _referenceEquals, new SpanPosition());
    //    //    //}

    //    //    //public SpanPosition SetReference(int first, int length, object element, SpanPosition spanPosition)
    //    //    //{
    //    //    //    return this.Set(first, length, element, _referenceEquals, spanPosition);
    //    //    //}

    //    //    //public void SetValue(int first, int length, object element)
    //    //    //{
    //    //    //    this.Set(first, length, element, _equals, new SpanPosition());
    //    //    //}

    //    //    //public SpanPosition SetValue(int first, int length, object element, SpanPosition spanPosition)
    //    //    //{
    //    //    //    return this.Set(first, length, element, _equals, spanPosition);
    //    //    //}

    //    //    // Properties
    //    //    public int Count
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._spans.Count;
    //    //        }
    //    //    }

    //    //    public object Default
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._defaultObject;
    //    //        }
    //    //    }

    //    //    public Span this[int index]
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._spans[index];
    //    //        }
    //    //    }
    //    //}

    //    internal sealed class Span
    //    {
    //        // Fields
    //        public object element;
    //        public int length;

    //        // Methods
    //        public Span(object element, int length)
    //        {
    //            this.element = element;
    //            this.length = length;
    //        }
    //    }

    //    //internal sealed class SpanEnumerator : IEnumerator
    //    //{
    //    //    // Fields
    //    //    private int _current;
    //    //    private SpanVector _spans;

    //    //    // Methods
    //    //    internal SpanEnumerator(SpanVector spans)
    //    //    {
    //    //        this._spans = spans;
    //    //        this._current = -1;
    //    //    }

    //    //    public bool MoveNext()
    //    //    {
    //    //        this._current++;
    //    //        if (this._current >= this._spans.Count)
    //    //        {
    //    //            return false;
    //    //        }
    //    //        return true;
    //    //    }

    //    //    public void Reset()
    //    //    {
    //    //        this._current = -1;
    //    //    }

    //    //    // Properties
    //    //    public object Current
    //    //    {
    //    //        get
    //    //        {
    //    //            return this._spans[this._current];
    //    //        }
    //    //    }
    //    //}

    //    //[StructLayout(LayoutKind.Sequential), FriendAccessAllowed]
    //    //internal struct FrugalStructList<T>
    //    //{
    //    //    internal FrugalListBase<T> _listStore;
    //    //    public FrugalStructList(int size)
    //    //    {
    //    //        this._listStore = null;
    //    //        this.Capacity = size;
    //    //    }

    //    //    public FrugalStructList(ICollection collection)
    //    //    {
    //    //        if (collection.Count > 6)
    //    //        {
    //    //            this._listStore = new ArrayItemList<T>(collection);
    //    //        }
    //    //        else
    //    //        {
    //    //            this._listStore = null;
    //    //            this.Capacity = collection.Count;
    //    //            foreach (T local in collection)
    //    //            {
    //    //                this.Add(local);
    //    //            }
    //    //        }
    //    //    }

    //    //    public FrugalStructList(ICollection<T> collection)
    //    //    {
    //    //        if (collection.get_Count() > 6)
    //    //        {
    //    //            this._listStore = new ArrayItemList<T>(collection);
    //    //        }
    //    //        else
    //    //        {
    //    //            this._listStore = null;
    //    //            this.Capacity = collection.get_Count();
    //    //            foreach (T local in collection)
    //    //            {
    //    //                this.Add(local);
    //    //            }
    //    //        }
    //    //    }

    //    //    public int Capacity
    //    //    {
    //    //        get
    //    //        {
    //    //            if (this._listStore != null)
    //    //            {
    //    //                return this._listStore.Capacity;
    //    //            }
    //    //            return 0;
    //    //        }
    //    //        set
    //    //        {
    //    //            int capacity = 0;
    //    //            if (this._listStore != null)
    //    //            {
    //    //                capacity = this._listStore.Capacity;
    //    //            }
    //    //            if (capacity < value)
    //    //            {
    //    //                FrugalListBase<T> base2;
    //    //                if (value == 1)
    //    //                {
    //    //                    base2 = new SingleItemList<T>();
    //    //                }
    //    //                else if (value <= 3)
    //    //                {
    //    //                    base2 = new ThreeItemList<T>();
    //    //                }
    //    //                else if (value <= 6)
    //    //                {
    //    //                    base2 = new SixItemList<T>();
    //    //                }
    //    //                else
    //    //                {
    //    //                    base2 = new ArrayItemList<T>(value);
    //    //                }
    //    //                if (this._listStore != null)
    //    //                {
    //    //                    base2.Promote(this._listStore);
    //    //                }
    //    //                this._listStore = base2;
    //    //            }
    //    //        }
    //    //    }
    //    //    public int Count
    //    //    {
    //    //        get
    //    //        {
    //    //            if (this._listStore != null)
    //    //            {
    //    //                return this._listStore.Count;
    //    //            }
    //    //            return 0;
    //    //        }
    //    //    }
    //    //    public T this[int index]
    //    //    {
    //    //        get
    //    //        {
    //    //            if (((this._listStore == null) || (index >= this._listStore.Count)) || (index < 0))
    //    //            {
    //    //                throw new ArgumentOutOfRangeException("index");
    //    //            }
    //    //            return this._listStore.EntryAt(index);
    //    //        }
    //    //        set
    //    //        {
    //    //            if (((this._listStore == null) || (index >= this._listStore.Count)) || (index < 0))
    //    //            {
    //    //                throw new ArgumentOutOfRangeException("index");
    //    //            }
    //    //            this._listStore.SetAt(index, value);
    //    //        }
    //    //    }
    //    //    public int Add(T value)
    //    //    {
    //    //        if (this._listStore == null)
    //    //        {
    //    //            this._listStore = new SingleItemList<T>();
    //    //        }
    //    //        FrugalListStoreState state = this._listStore.Add(value);
    //    //        if (state != FrugalListStoreState.Success)
    //    //        {
    //    //            if (FrugalListStoreState.ThreeItemList != state)
    //    //            {
    //    //                if (FrugalListStoreState.SixItemList != state)
    //    //                {
    //    //                    if (FrugalListStoreState.Array != state)
    //    //                    {
    //    //                        throw new InvalidOperationException(SR.Get("FrugalList_CannotPromoteBeyondArray"));
    //    //                    }
    //    //                    ArrayItemList<T> list3 = new ArrayItemList<T>(this._listStore.Count + 1);
    //    //                    list3.Promote(this._listStore);
    //    //                    this._listStore = list3;
    //    //                    list3.Add(value);
    //    //                    this._listStore = list3;
    //    //                }
    //    //                else
    //    //                {
    //    //                    SixItemList<T> list2 = new SixItemList<T>();
    //    //                    list2.Promote(this._listStore);
    //    //                    this._listStore = list2;
    //    //                    list2.Add(value);
    //    //                    this._listStore = list2;
    //    //                }
    //    //            }
    //    //            else
    //    //            {
    //    //                ThreeItemList<T> list = new ThreeItemList<T>();
    //    //                list.Promote(this._listStore);
    //    //                list.Add(value);
    //    //                this._listStore = list;
    //    //            }
    //    //        }
    //    //        return (this._listStore.Count - 1);
    //    //    }

    //    //    public void Clear()
    //    //    {
    //    //        if (this._listStore != null)
    //    //        {
    //    //            this._listStore.Clear();
    //    //        }
    //    //    }

    //    //    public bool Contains(T value)
    //    //    {
    //    //        return (((this._listStore != null) && (this._listStore.Count > 0)) && this._listStore.Contains(value));
    //    //    }

    //    //    public int IndexOf(T value)
    //    //    {
    //    //        if ((this._listStore != null) && (this._listStore.Count > 0))
    //    //        {
    //    //            return this._listStore.IndexOf(value);
    //    //        }
    //    //        return -1;
    //    //    }

    //    //    public void Insert(int index, T value)
    //    //    {
    //    //        if ((index != 0) && (((this._listStore == null) || (index > this._listStore.Count)) || (index < 0)))
    //    //        {
    //    //            throw new ArgumentOutOfRangeException("index");
    //    //        }
    //    //        int num = 1;
    //    //        if ((this._listStore != null) && (this._listStore.Count == this._listStore.Capacity))
    //    //        {
    //    //            num = this.Capacity + 1;
    //    //        }
    //    //        this.Capacity = num;
    //    //        this._listStore.Insert(index, value);
    //    //    }

    //    //    public bool Remove(T value)
    //    //    {
    //    //        return (((this._listStore != null) && (this._listStore.Count > 0)) && this._listStore.Remove(value));
    //    //    }

    //    //    public void RemoveAt(int index)
    //    //    {
    //    //        if (((this._listStore == null) || (index >= this._listStore.Count)) || (index < 0))
    //    //        {
    //    //            throw new ArgumentOutOfRangeException("index");
    //    //        }
    //    //        this._listStore.RemoveAt(index);
    //    //    }

    //    //    public void EnsureIndex(int index)
    //    //    {
    //    //        if (index < 0)
    //    //        {
    //    //            throw new ArgumentOutOfRangeException("index");
    //    //        }
    //    //        int num = (index + 1) - this.Count;
    //    //        if (num > 0)
    //    //        {
    //    //            this.Capacity = index + 1;
    //    //            T local = default(T);
    //    //            for (int i = 0; i < num; i++)
    //    //            {
    //    //                this._listStore.Add(local);
    //    //            }
    //    //        }
    //    //    }

    //    //    public T[] ToArray()
    //    //    {
    //    //        if ((this._listStore != null) && (this._listStore.Count > 0))
    //    //        {
    //    //            return this._listStore.ToArray();
    //    //        }
    //    //        return null;
    //    //    }

    //    //    public void CopyTo(T[] array, int index)
    //    //    {
    //    //        if ((this._listStore != null) && (this._listStore.Count > 0))
    //    //        {
    //    //            this._listStore.CopyTo(array, index);
    //    //        }
    //    //    }

    //    //    public FrugalStructList<T> Clone()
    //    //    {
    //    //        FrugalStructList<T> list = new FrugalStructList<T>();
    //    //        if (this._listStore != null)
    //    //        {
    //    //            list._listStore = (FrugalListBase<T>)this._listStore.Clone();
    //    //        }
    //    //        return list;
    //    //    }
    //    //}

    //    internal sealed class GenericTextParagraphProperties : TextParagraphProperties
    //    {
    //        // Fields
    //        private bool _alwaysCollapsible;
    //        private TextRunProperties _defaultTextRunProperties;
    //        private bool _firstLineInParagraph;
    //        private FlowDirection _flowDirection;
    //        private double _indent;
    //        private double _lineHeight;
    //        private TextAlignment _textAlignment;
    //        private TextWrapping _textWrap;

    //        // Methods
    //        public GenericTextParagraphProperties(TextParagraphProperties textParagraphProperties)
    //        {
    //            this._flowDirection = textParagraphProperties.FlowDirection;
    //            this._defaultTextRunProperties = textParagraphProperties.DefaultTextRunProperties;
    //            this._textAlignment = textParagraphProperties.TextAlignment;
    //            this._lineHeight = textParagraphProperties.LineHeight;
    //            this._firstLineInParagraph = textParagraphProperties.FirstLineInParagraph;
    //            this._alwaysCollapsible = textParagraphProperties.AlwaysCollapsible;
    //            this._textWrap = textParagraphProperties.TextWrapping;
    //            this._indent = textParagraphProperties.Indent;
    //        }

    //        public GenericTextParagraphProperties(FlowDirection flowDirection, TextAlignment textAlignment, bool firstLineInParagraph, bool alwaysCollapsible, TextRunProperties defaultTextRunProperties, TextWrapping textWrap, double lineHeight, double indent)
    //        {
    //            this._flowDirection = flowDirection;
    //            this._textAlignment = textAlignment;
    //            this._firstLineInParagraph = firstLineInParagraph;
    //            this._alwaysCollapsible = alwaysCollapsible;
    //            this._defaultTextRunProperties = defaultTextRunProperties;
    //            this._textWrap = textWrap;
    //            this._lineHeight = lineHeight;
    //            this._indent = indent;
    //        }

    //        internal void SetFlowDirection(FlowDirection flowDirection)
    //        {
    //            this._flowDirection = flowDirection;
    //        }

    //        internal void SetLineHeight(double lineHeight)
    //        {
    //            this._lineHeight = lineHeight;
    //        }

    //        internal void SetTextAlignment(TextAlignment textAlignment)
    //        {
    //            this._textAlignment = textAlignment;
    //        }

    //        internal void SetTextWrapping(TextWrapping textWrap)
    //        {
    //            this._textWrap = textWrap;
    //        }

    //        // Properties
    //        public override bool AlwaysCollapsible
    //        {
    //            get
    //            {
    //                return this._alwaysCollapsible;
    //            }
    //        }

    //        public override TextRunProperties DefaultTextRunProperties
    //        {
    //            get
    //            {
    //                return this._defaultTextRunProperties;
    //            }
    //        }

    //        public override bool FirstLineInParagraph
    //        {
    //            get
    //            {
    //                return this._firstLineInParagraph;
    //            }
    //        }

    //        public override FlowDirection FlowDirection
    //        {
    //            get
    //            {
    //                return this._flowDirection;
    //            }
    //        }

    //        public override double Indent
    //        {
    //            get
    //            {
    //                return this._indent;
    //            }
    //        }

    //        public override double LineHeight
    //        {
    //            get
    //            {
    //                return this._lineHeight;
    //            }
    //        }

    //        public override TextAlignment TextAlignment
    //        {
    //            get
    //            {
    //                return this._textAlignment;
    //            }
    //        }

    //        public override TextMarkerProperties TextMarkerProperties
    //        {
    //            get
    //            {
    //                return null;
    //            }
    //        }

    //        public override TextWrapping TextWrapping
    //        {
    //            get
    //            {
    //                return this._textWrap;
    //            }
    //        }
    //    }

    //    internal sealed class GenericTextRunProperties : TextRunProperties
    //    {
    //        // Fields
    //        private Brush _backgroundBrush;
    //        private BaselineAlignment _baselineAlignment;
    //        private CultureInfo _culture;
    //        private double _emHintingSize;
    //        private double _emSize;
    //        private Brush _foregroundBrush;
    //        private NumberSubstitution _numberSubstitution;
    //        private TextDecorationCollection _textDecorations;
    //        private Typeface _typeface;

    //        // Methods
    //        public GenericTextRunProperties(Typeface typeface, double size, double hintingSize, TextDecorationCollection textDecorations, Brush foregroundBrush, Brush backgroundBrush, BaselineAlignment baselineAlignment, CultureInfo culture, NumberSubstitution substitution)
    //        {
    //            this._typeface = typeface;
    //            this._emSize = size;
    //            this._emHintingSize = hintingSize;
    //            this._textDecorations = textDecorations;
    //            this._foregroundBrush = foregroundBrush;
    //            this._backgroundBrush = backgroundBrush;
    //            this._baselineAlignment = baselineAlignment;
    //            this._culture = culture;
    //            this._numberSubstitution = substitution;
    //        }

    //        public override bool Equals(object o)
    //        {
    //            if ((o == null) || !(o is TextRunProperties))
    //            {
    //                return false;
    //            }
    //            TextRunProperties properties = (TextRunProperties)o;
    //            if (((((((this._emSize != properties.FontRenderingEmSize) || (this._emHintingSize != properties.FontHintingEmSize)) || ((this._culture != properties.CultureInfo) || !this._typeface.Equals(properties.Typeface))) || !((this._textDecorations == null) ? (properties.TextDecorations == null) : this._textDecorations.ValueEquals(properties.TextDecorations))) || (this._baselineAlignment != properties.BaselineAlignment)) || !((this._foregroundBrush == null) ? (properties.ForegroundBrush == null) : this._foregroundBrush.Equals(properties.ForegroundBrush))) || !((this._backgroundBrush == null) ? (properties.BackgroundBrush == null) : this._backgroundBrush.Equals(properties.BackgroundBrush)))
    //            {
    //                return false;
    //            }
    //            if (this._numberSubstitution != null)
    //            {
    //                return this._numberSubstitution.Equals(properties.NumberSubstitution);
    //            }
    //            return (properties.NumberSubstitution == null);
    //        }

    //        public override int GetHashCode()
    //        {
    //            return ((((((((this._typeface.GetHashCode() ^ this._emSize.GetHashCode()) ^ this._emHintingSize.GetHashCode()) ^ ((this._foregroundBrush == null) ? 0 : this._foregroundBrush.GetHashCode())) ^ ((this._backgroundBrush == null) ? 0 : this._backgroundBrush.GetHashCode())) ^ ((this._textDecorations == null) ? 0 : this._textDecorations.GetHashCode())) ^ (((int)this._baselineAlignment) << 3)) ^ (this._culture.GetHashCode() << 6)) ^ ((this._numberSubstitution == null) ? 0 : this._numberSubstitution.GetHashCode()));
    //        }

    //        // Properties
    //        public override Brush BackgroundBrush
    //        {
    //            get
    //            {
    //                return this._backgroundBrush;
    //            }
    //        }

    //        public override BaselineAlignment BaselineAlignment
    //        {
    //            get
    //            {
    //                return this._baselineAlignment;
    //            }
    //        }

    //        public override CultureInfo CultureInfo
    //        {
    //            get
    //            {
    //                return this._culture;
    //            }
    //        }

    //        public override double FontHintingEmSize
    //        {
    //            get
    //            {
    //                return this._emHintingSize;
    //            }
    //        }

    //        public override double FontRenderingEmSize
    //        {
    //            get
    //            {
    //                return this._emSize;
    //            }
    //        }

    //        public override Brush ForegroundBrush
    //        {
    //            get
    //            {
    //                return this._foregroundBrush;
    //            }
    //        }

    //        public override NumberSubstitution NumberSubstitution
    //        {
    //            get
    //            {
    //                return this._numberSubstitution;
    //            }
    //        }

    //        public override TextDecorationCollection TextDecorations
    //        {
    //            get
    //            {
    //                return this._textDecorations;
    //            }
    //        }

    //        public override TextEffectCollection TextEffects
    //        {
    //            get
    //            {
    //                return null;
    //            }
    //        }

    //        public override Typeface Typeface
    //        {
    //            get
    //            {
    //                return this._typeface;
    //            }
    //        }

    //        public override TextRunTypographyProperties TypographyProperties
    //        {
    //            get
    //            {
    //                return null;
    //            }
    //        }
    //    }


    //    public abstract class TextFormatter : IDisposable
    //    {
    //        // Fields
    //        private static object _staticLock = new object();

    //        // Methods
    //        protected TextFormatter()
    //        {
    //        }

    //        public static TextFormatter Create()
    //        {
    //            return new TextFormatterImp();
    //        }

    //        //[FriendAccessAllowed]
    //        internal static TextFormatter CreateFromContext(TextFormatterContext soleContext)
    //        {
    //            return new TextFormatterImp(soleContext);
    //        }

    //        //[FriendAccessAllowed]
    //        internal abstract TextParagraphCache CreateParagraphCache(TextSource textSource, int firstCharIndex, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak, TextRunCache textRunCache);
    //        public virtual void Dispose()
    //        {
    //        }

    //        public abstract TextLine FormatLine(TextSource textSource, int firstCharIndex, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak);
    //        public abstract TextLine FormatLine(TextSource textSource, int firstCharIndex, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak, TextRunCache textRunCache);
    //        public abstract MinMaxParagraphWidth FormatMinMaxParagraphWidth(TextSource textSource, int firstCharIndex, TextParagraphProperties paragraphProperties);
    //        public abstract MinMaxParagraphWidth FormatMinMaxParagraphWidth(TextSource textSource, int firstCharIndex, TextParagraphProperties paragraphProperties, TextRunCache textRunCache);
    //        //[FriendAccessAllowed]
    //        internal static TextFormatter FromCurrentDispatcher()
    //        {
    //            Dispatcher currentDispatcher = Dispatcher.CurrentDispatcher;
    //            if (currentDispatcher == null)
    //            {
    //                //throw new ArgumentException(SR.Get("CurrentDispatcherNotFound"));
    //            }



    //            TextFormatter formatter = (TextFormatterImp)currentDispatcher.Reserved1;
    //            if (formatter == null)
    //            {
    //                lock (_staticLock)
    //                {
    //                    if (formatter == null)
    //                    {
    //                        formatter = Create();
    //                        currentDispatcher.Reserved1 = formatter;
    //                    }
    //                }
    //            }
    //            //Invariant.Assert(formatter != null);
    //            return formatter;
    //        }

    //        //[FriendAccessAllowed]
    //        internal abstract TextLine RecreateLine(TextSource textSource, int firstCharIndex, int lineLength, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak, TextRunCache textRunCache);
    //    }

    //    internal sealed class TextFormatterImp : TextFormatter
    //    {
    //        // Fields
    //        private FrugalStructList<TextFormatterContext> _contextList;
    //        private GlyphingCache _glyphingCache;
    //        private bool _multipleContextProhibited;
    //        private double _toIdeal;
    //        private double _toReal;
    //        private const int MaxGlyphingCacheCapacity = 0x10;

    //        // Methods
    //        internal TextFormatterImp()
    //            : this(null)
    //        {
    //        }

    //        internal TextFormatterImp(TextFormatterContext soleContext)
    //        {
    //            this._toIdeal = 300.0;
    //            this._toReal = 0.0033333333333333335;
    //            if (soleContext != null)
    //            {
    //                this._contextList.Add(soleContext);
    //            }
    //            this._multipleContextProhibited = this._contextList.Count != 0;
    //        }

    //        [SecurityTreatAsSafe, SecurityCritical]
    //        internal TextFormatterContext AcquireContext(object owner, IntPtr ploc)
    //        {
    //            Invariant.Assert(owner != null);
    //            TextFormatterContext context = null;
    //            int count = this._contextList.Count;
    //            int num = 0;
    //            while (num < count)
    //            {
    //                context = this._contextList[num];
    //                if (ploc == IntPtr.Zero)
    //                {
    //                    if (context.Owner == null)
    //                    {
    //                        break;
    //                    }
    //                }
    //                else if (ploc == context.Ploc.Value)
    //                {
    //                    break;
    //                }
    //                num++;
    //            }
    //            if (num == count)
    //            {
    //                if ((count != 0) && this._multipleContextProhibited)
    //                {
    //                    throw new InvalidOperationException(SR.Get("TextFormatterReentranceProhibited"));
    //                }
    //                context = new TextFormatterContext();
    //                this._contextList.Add(context);
    //            }
    //            context.Owner = owner;
    //            return context;
    //        }

    //        private void CleanupInternal()
    //        {
    //            for (int i = 0; i < this._contextList.Count; i++)
    //            {
    //                this._contextList[i].Destroy();
    //            }
    //            this._contextList.Clear();
    //        }

    //        internal static MatrixTransform CreateAntiInversionTransform(InvertAxes inversion, double paragraphWidth, double lineHeight)
    //        {
    //            if (inversion == InvertAxes.None)
    //            {
    //                return null;
    //            }
    //            double num2 = 1.0;
    //            double num = 1.0;
    //            double offsetX = 0.0;
    //            double offsetY = 0.0;
    //            if ((inversion & InvertAxes.Horizontal) != InvertAxes.None)
    //            {
    //                num2 = -num2;
    //                offsetX = paragraphWidth;
    //            }
    //            if ((inversion & InvertAxes.Vertical) != InvertAxes.None)
    //            {
    //                num = -num;
    //                offsetY = lineHeight;
    //            }
    //            return new MatrixTransform(num2, 0.0, 0.0, num, offsetX, offsetY);
    //        }

    //        internal override TextParagraphCache CreateParagraphCache(TextSource textSource, int firstCharIndex, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak, TextRunCache textRunCache)
    //        {
    //            FormatSettings settings = this.PrepareFormatSettings(textSource, firstCharIndex, paragraphWidth, paragraphProperties, previousLineBreak, textRunCache, true, false);
    //            if (!settings.Pap.Wrap && settings.Pap.OptimalBreak)
    //            {
    //                throw new ArgumentException(SR.Get("OptimalParagraphMustWrap"));
    //            }
    //            return new TextParagraphCache(settings, firstCharIndex, this.RealToIdeal(paragraphWidth));
    //        }

    //        public override void Dispose()
    //        {
    //            this.CleanupInternal();
    //            base.Dispose();
    //            GC.SuppressFinalize(this);
    //        }

    //        ~TextFormatterImp()
    //        {
    //            this.CleanupInternal();
    //        }

    //        public override TextLine FormatLine(TextSource textSource, int firstCharIndex, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak)
    //        {
    //            return this.FormatLineInternal(textSource, firstCharIndex, 0, paragraphWidth, paragraphProperties, previousLineBreak, new TextRunCache());
    //        }

    //        public override TextLine FormatLine(TextSource textSource, int firstCharIndex, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak, TextRunCache textRunCache)
    //        {
    //            return this.FormatLineInternal(textSource, firstCharIndex, 0, paragraphWidth, paragraphProperties, previousLineBreak, textRunCache);
    //        }

    //        private TextLine FormatLineInternal(TextSource textSource, int firstCharIndex, int lineLength, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak, TextRunCache textRunCache)
    //        {
    //            if (EventTrace.IsEnabled(EventTrace.Flags.performance, EventTrace.Level.verbose))
    //            {
    //                EventTrace.EventProvider.TraceEvent(EventTrace.GuidFromId(EventTraceGuidId.GENERICSTRINGGUID), 1, "TextFormatterImp.FormatLineInternal Start");
    //            }
    //            FormatSettings settings = this.PrepareFormatSettings(textSource, firstCharIndex, paragraphWidth, paragraphProperties, previousLineBreak, textRunCache, lineLength != 0, true);
    //            TextLine line = null;
    //            if ((!settings.Pap.AlwaysCollapsible && (previousLineBreak == null)) && (lineLength <= 0))
    //            {
    //                line = SimpleTextLine.Create(settings, firstCharIndex, this.RealToIdealFloor(paragraphWidth));
    //            }
    //            if (line == null)
    //            {
    //                line = new TextMetrics.FullTextLine(settings, firstCharIndex, lineLength, this.RealToIdealFloor(paragraphWidth), LineFlags.None);
    //            }
    //            if (EventTrace.IsEnabled(EventTrace.Flags.performance, EventTrace.Level.verbose))
    //            {
    //                EventTrace.EventProvider.TraceEvent(EventTrace.GuidFromId(EventTraceGuidId.GENERICSTRINGGUID), 2, "TextFormatterImp.FormatLineInternal End");
    //            }
    //            return line;
    //        }

    //        public override MinMaxParagraphWidth FormatMinMaxParagraphWidth(TextSource textSource, int firstCharIndex, TextParagraphProperties paragraphProperties)
    //        {
    //            return this.FormatMinMaxParagraphWidth(textSource, firstCharIndex, paragraphProperties, new TextRunCache());
    //        }

    //        public override MinMaxParagraphWidth FormatMinMaxParagraphWidth(TextSource textSource, int firstCharIndex, TextParagraphProperties paragraphProperties, TextRunCache textRunCache)
    //        {
    //            TextMetrics.FullTextLine line = new TextMetrics.FullTextLine(this.PrepareFormatSettings(textSource, firstCharIndex, 0.0, paragraphProperties, null, textRunCache, false, true), firstCharIndex, 0, 0, LineFlags.KeepState | LineFlags.MinMax);
    //            MinMaxParagraphWidth width = new MinMaxParagraphWidth(line.MinWidth, line.Width);
    //            line.Dispose();
    //            return width;
    //        }

    //        internal double IdealToReal(int i)
    //        {
    //            return (i * this._toReal);
    //        }

    //        private FormatSettings PrepareFormatSettings(TextSource textSource, int firstCharIndex, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak, TextRunCache textRunCache, bool useOptimalBreak, bool isSingleLineFormatting)
    //        {
    //            this.VerifyTextFormattingArguments(textSource, firstCharIndex, paragraphWidth, paragraphProperties, textRunCache);
    //            if (textRunCache.Imp == null)
    //            {
    //                textRunCache.Imp = new TextRunCacheImp();
    //            }
    //            return new FormatSettings(this, textSource, textRunCache.Imp, new ParaProp(this, paragraphProperties, useOptimalBreak), previousLineBreak, isSingleLineFormatting);
    //        }

    //        internal int RealToIdeal(double i)
    //        {
    //            return (int)Math.Round((double)(i * this._toIdeal));
    //        }

    //        internal int RealToIdealFloor(double i)
    //        {
    //            return (int)Math.Floor(i * this._toIdeal);
    //        }

    //        internal override TextLine RecreateLine(TextSource textSource, int firstCharIndex, int lineLength, double paragraphWidth, TextParagraphProperties paragraphProperties, TextLineBreak previousLineBreak, TextRunCache textRunCache)
    //        {
    //            return this.FormatLineInternal(textSource, firstCharIndex, lineLength, paragraphWidth, paragraphProperties, previousLineBreak, textRunCache);
    //        }

    //        internal static void VerifyCaretCharacterHit(CharacterHit characterHit, int cpFirst, int cchLength)
    //        {
    //            //if ((characterHit.FirstCharacterIndex < cpFirst) || (characterHit.FirstCharacterIndex > (cpFirst + cchLength)))
    //            //{
    //            //    throw new ArgumentOutOfRangeException("First", SR.Get("ParameterMustBeBetween", new object[] { cpFirst, cpFirst + cchLength }));
    //            //}
    //            //if (characterHit.TrailingLength < 0)
    //            //{
    //            //    throw new ArgumentOutOfRangeException("Length", SR.Get("ParameterCannotBeNegative"));
    //            //}
    //        }

    //        private void VerifyTextFormattingArguments(TextSource textSource, int firstCharIndex, double paragraphWidth, TextParagraphProperties paragraphProperties, TextRunCache textRunCache)
    //        {
    //            //if (textSource == null)
    //            //{
    //            //    throw new ArgumentNullException("textSource");
    //            //}
    //            //if (textRunCache == null)
    //            //{
    //            //    throw new ArgumentNullException("textRunCache");
    //            //}
    //            //if (paragraphProperties == null)
    //            //{
    //            //    throw new ArgumentNullException("paragraphProperties");
    //            //}
    //            //if (paragraphProperties.DefaultTextRunProperties == null)
    //            //{
    //            //    throw new ArgumentNullException("paragraphProperties.DefaultTextRunProperties");
    //            //}
    //            //if (paragraphProperties.DefaultTextRunProperties.Typeface == null)
    //            //{
    //            //    throw new ArgumentNullException("paragraphProperties.DefaultTextRunProperties.Typeface");
    //            //}
    //            //if (DoubleUtil.IsNaN(paragraphWidth))
    //            //{
    //            //    throw new ArgumentOutOfRangeException("paragraphWidth", SR.Get("ParameterValueCannotBeNaN"));
    //            //}
    //            //if (double.IsInfinity(paragraphWidth))
    //            //{
    //            //    throw new ArgumentOutOfRangeException("paragraphWidth", SR.Get("ParameterValueCannotBeInfinity"));
    //            //}
    //            //double num = this.IdealToReal(0x3ffffffe);
    //            //if ((paragraphWidth < 0.0) || (paragraphWidth > num))
    //            //{
    //            //    throw new ArgumentOutOfRangeException("paragraphWidth", SR.Get("ParameterMustBeBetween", new object[] { 0, num }));
    //            //}
    //            //double num2 = num / 100.0;
    //            //if ((paragraphProperties.DefaultTextRunProperties.FontRenderingEmSize < 0.0) || (paragraphProperties.DefaultTextRunProperties.FontRenderingEmSize > num2))
    //            //{
    //            //    throw new ArgumentOutOfRangeException("paragraphProperties.DefaultTextRunProperties.FontRenderingEmSize", SR.Get("ParameterMustBeBetween", new object[] { 0, num2 }));
    //            //}
    //            //if (paragraphProperties.Indent > num)
    //            //{
    //            //    throw new ArgumentOutOfRangeException("paragraphProperties.Indent", SR.Get("ParameterCannotBeGreaterThan", new object[] { num }));
    //            //}
    //            //if (paragraphProperties.LineHeight > num)
    //            //{
    //            //    throw new ArgumentOutOfRangeException("paragraphProperties.LineHeight", SR.Get("ParameterCannotBeGreaterThan", new object[] { num }));
    //            //}
    //            //if ((paragraphProperties.DefaultIncrementalTab < 0.0) || (paragraphProperties.DefaultIncrementalTab > num))
    //            //{
    //            //    throw new ArgumentOutOfRangeException("paragraphProperties.DefaultIncrementalTab", SR.Get("ParameterMustBeBetween", new object[] { 0, num }));
    //            //}
    //        }

    //        // Properties
    //        internal GlyphingCache GlyphingCache
    //        {
    //            get
    //            {
    //                if (this._glyphingCache == null)
    //                {
    //                    this._glyphingCache = new GlyphingCache(0x10);
    //                }
    //                return this._glyphingCache;
    //            }
    //        }

    //        internal double ToIdeal
    //        {
    //            get
    //            {
    //                return this._toIdeal;
    //            }
    //        }

    //        internal double ToReal
    //        {
    //            get
    //            {
    //                return this._toReal;
    //            }
    //        }
    //    }


    //}
}

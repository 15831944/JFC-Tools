// JFCGridControl.JFCGrid
using JFCGridControl;
using System;
using System.CodeDom.Compiler;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Linq.Expressions;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Threading;



namespace JFCGridControl
{
    public partial class JFCGrid : UserControl
    {
        public delegate void DependencyPropertyLineOverChangedEventHandler(object sender, DependencyPropertyLineOverChangedEventArgs e);

        public struct DependencyPropertyLineOverChangedEventArgs
        {
            public enum GridPosition
            {
                HeaderRow,
                BodyRow,
                HeaderFooterRow,
                BodyFooterRow,
                None
            }

            private int _IndexDataRow;

            private GridPosition _Position;

            private object _NewValue;

            private object _OldValue;

            private DependencyProperty _Property;

            public int IndexDataRow
            {
                get
                {
                    return this._IndexDataRow;
                }
            }

            public GridPosition Position
            {
                get
                {
                    return this._Position;
                }
            }

            public object NewValue
            {
                get
                {
                    return this._NewValue;
                }
            }

            public object OldValue
            {
                get
                {
                    return this._OldValue;
                }
            }

            public DependencyProperty Property
            {
                get
                {
                    return this._Property;
                }
            }

            public DependencyPropertyLineOverChangedEventArgs(DependencyProperty property, object oldValue, object newValue, GridPosition Position, int IndexDataRow)
            {
                this._Property = property;
                this._OldValue = oldValue;
                this._NewValue = newValue;
                this._Position = Position;
                this._IndexDataRow = IndexDataRow;
            }

            public static bool operator !=(DependencyPropertyLineOverChangedEventArgs left, DependencyPropertyLineOverChangedEventArgs right)
            {
                if (left == right)
                {
                    return false;
                }
                return true;
            }

            public static bool operator ==(DependencyPropertyLineOverChangedEventArgs left, DependencyPropertyLineOverChangedEventArgs right)
            {
                if (left == right)
                {
                    return true;
                }
                return false;
            }

            public bool Equals(DependencyPropertyLineOverChangedEventArgs args)
            {
                if (this.NewValue == args.NewValue && this.OldValue == args.OldValue && this.Property == args.Property)
                {
                    return true;
                }
                return false;
            }

            public override bool Equals(object obj)
            {
                if (obj is DependencyPropertyLineOverChangedEventArgs)
                {
                    DependencyPropertyLineOverChangedEventArgs args = (DependencyPropertyLineOverChangedEventArgs)obj;
                    if (this.Equals(args))
                    {
                        return true;
                    }
                }
                return false;
            }

            public override int GetHashCode()
            {
                return this.NewValue.GetHashCode() + this.OldValue.GetHashCode();
            }
        }

        public struct GridPosition
        {
            private object _Row;

            private IEnumerable<object> _Path;

            private JFCGridColumn _Column;

            private JFCExpendItem _Item;

            public object Row
            {
                get
                {
                    return this._Row;
                }
            }

            public IEnumerable<object> Path
            {
                get
                {
                    return this._Path;
                }
            }

            public JFCGridColumn Column
            {
                get
                {
                    return this._Column;
                }
            }

            internal JFCExpendItem Item
            {
                get
                {
                    return this._Item;
                }
                set
                {
                    this._Item = value;
                }
            }

            public GridPosition(object Row, IEnumerable<object> Path, JFCGridColumn Column)
            {
                this._Row = Row;
                this._Path = Path;
                this._Column = Column;
                this._Item = null;
            }

            internal void SetItem(JFCExpendItem item)
            {
                this._Item = item;
            }
        }

        public delegate void DependencyPropertySearchChangedEventHandler(object sender, DependencyPropertySearchChangedEventArgs e);

        public struct DependencyPropertySearchChangedEventArgs
        {
            private string _Text;

            private JFCGridControl.ObservableCollection<GridPosition> _Result;

            public string Text
            {
                get
                {
                    return this._Text;
                }
            }

            public JFCGridControl.ObservableCollection<GridPosition> Result
            {
                get
                {
                    return this._Result;
                }
            }

            public DependencyPropertySearchChangedEventArgs(string textSearching, JFCGridControl.ObservableCollection<GridPosition> result)
            {
                this._Text = textSearching;
                this._Result = result;
            }
        }

        [ValueConversion(typeof(bool), typeof(bool))]
        public class BooleanConverter : IValueConverter
        {
            public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
            {
                return value;
            }

            public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
            {
                return value;
            }
        }

        private Size MaxSize;

        internal JFCGridManager Manager;

        internal JFCExpendItem[] dataSource;

        internal JFCExpendItem[] dataSourceFooter;

        private HashSet<JFCExpendItem> selectedExpendItems = new HashSet<JFCExpendItem>();

        private HashSet<JFCExpendItem> selectedExpendItemsChildren = new HashSet<JFCExpendItem>();

        private int scrollVerticalValue;

        private int scrollHorizontalValue;

        public JFCGridControl.ObservableCollection<JFCGridColumn> ColunmDefinition = new JFCGridControl.ObservableCollection<JFCGridColumn>();

        public JFCGridControl.ObservableCollection<JFCGridColumn> ColunmDefinitionFooter = new JFCGridControl.ObservableCollection<JFCGridColumn>();

        private bool isVirtualised = true;

        public static readonly DependencyProperty RowHeightProperty = DependencyProperty.Register("RowHeight", typeof(int), typeof(JFCGrid), new UIPropertyMetadata(18, JFCGrid.UpdateRowHeight));

        public static readonly DependencyProperty DataSourceProperty = DependencyProperty.Register("DataSource", typeof(IEnumerable), typeof(JFCGrid), new UIPropertyMetadata(null, JFCGrid.UpdateDatasource));

        public static readonly DependencyProperty DataSourceFooterProperty = DependencyProperty.Register("DataSourceFooter", typeof(IEnumerable), typeof(JFCGrid), new UIPropertyMetadata(null, JFCGrid.UpdateDatasourceFooter));

        public static readonly DependencyProperty PropertyChildrenNameProperty = DependencyProperty.Register("PropertyChildrenName", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("", JFCGrid.UpdateChildrenName));

        public static readonly DependencyProperty PropertyExpandeChildrenNameProperty = DependencyProperty.Register("PropertyExpandeChildrenName", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("", JFCGrid.UpdateExpandeChildrenName));

        public static readonly DependencyProperty HorizontalBorderColorProperty = DependencyProperty.Register("HorizontalBorderColor", typeof(Brush), typeof(JFCGrid), new UIPropertyMetadata(Brushes.Transparent, JFCGrid.UpdateHorizontalBorderColor));

        public static readonly DependencyProperty VerticalBorderColorProperty = DependencyProperty.Register("VerticalBorderColor", typeof(Brush), typeof(JFCGrid), new UIPropertyMetadata(Brushes.Transparent, JFCGrid.UpdateVerticalBorderColor));

        public static readonly DependencyProperty ScrollViewerBorderColorProperty = DependencyProperty.Register("ScrollViewerBorderColor", typeof(Brush), typeof(JFCGrid), new UIPropertyMetadata(Brushes.Black));

        public static readonly DependencyProperty ScrollViewerBorderThicknessProperty = DependencyProperty.Register("ScrollViewerBorderThickness", typeof(Thickness), typeof(JFCGrid), new UIPropertyMetadata(new Thickness(0.0)));

        public static readonly DependencyProperty ScrollViewerBackgroundColorProperty = DependencyProperty.Register("ScrollViewerBackgroundColor", typeof(Brush), typeof(JFCGrid), new UIPropertyMetadata(Brushes.White));

        public static readonly DependencyProperty ScrollViewerBackgroundColorHeaderColumnProperty = DependencyProperty.Register("ScrollViewerBackgroundColorHeaderColumn", typeof(Brush), typeof(JFCGrid), new PropertyMetadata(Brushes.Transparent));

        public static readonly DependencyProperty ContextmenuRowProperty = DependencyProperty.Register("ContextmenuRow", typeof(ContextMenu), typeof(JFCGrid), new UIPropertyMetadata(null, JFCGrid.UpdateContextMenuRow));

        public static readonly DependencyProperty ColumnMovableProperty = DependencyProperty.Register("ColumnMovable", typeof(bool), typeof(JFCGrid), new UIPropertyMetadata(false));

        public static readonly DependencyProperty NbRowMaxProperty = DependencyProperty.Register("NbRowMax", typeof(int), typeof(JFCGrid), new UIPropertyMetadata(30));

        public static readonly DependencyProperty ScrollBarVerticalVisibilityProperty = DependencyProperty.Register("ScrollBarVerticalVisibility", typeof(Visibility), typeof(JFCGrid), new UIPropertyMetadata(Visibility.Visible));

        public static readonly DependencyProperty ScrollBarHorizontalVisibilityProperty = DependencyProperty.Register("ScrollBarHorizontalVisibility", typeof(Visibility), typeof(JFCGrid), new UIPropertyMetadata(Visibility.Visible));

        public static readonly DependencyProperty LabelResearchProperty = DependencyProperty.Register("LabelResearch", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("Recherche"));

        public static readonly DependencyProperty LabelResultProperty = DependencyProperty.Register("LabelResult", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("Résultat"));

        public static readonly DependencyProperty HiliteStringProperty = DependencyProperty.Register("HiliteString", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("", JFCGrid.UpdateHiliteString));

        public static readonly DependencyProperty EcartHeaderColumnProperty = DependencyProperty.Register("EcartHeaderColumn", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4.0), JFCGrid.UpdateEcart));

        public static readonly DependencyProperty EcartHeaderRowProperty = DependencyProperty.Register("EcartHeaderRow", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4.0), JFCGrid.UpdateEcart));

        public static readonly DependencyProperty EcartBodyFooterHorizontalProperty = DependencyProperty.Register("EcartBodyFooterHorizontal", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4.0), JFCGrid.UpdateEcart));

        public static readonly DependencyProperty EcartBodyFooterVerticalProperty = DependencyProperty.Register("EcartBodyFooterVertical", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4.0), JFCGrid.UpdateEcart));

        public static readonly DependencyProperty EcartScrollHorizontalProperty = DependencyProperty.Register("EcartScrollHorizontal", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4.0), JFCGrid.UpdateEcart));

        public static readonly DependencyProperty EcartScrollVerticalProperty = DependencyProperty.Register("EcartScrollVertical", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4.0), JFCGrid.UpdateEcart));

        internal JFCGridControl.ObservableCollection<GridPosition> SearchResult = new JFCGridControl.ObservableCollection<GridPosition>();

        private int IndexSearch = -1;

        public static readonly DependencyProperty IsPossibleToSearchProperty = DependencyProperty.Register("IsPossibleToSearch", typeof(bool), typeof(JFCGrid), new UIPropertyMetadata(false, JFCGrid.UpdateSearchButtonVisibility));

        private List<int> LstGroupingNoRowLevel = new List<int>();

        private List<int> LstGroupingWithRowLevel = new List<int>();

        private List<int> LstGroupingWithRowWhenNoChildrenLevel = new List<int>();

        private static Dictionary<Type, Func<object, IEnumerable>> propGetter = new Dictionary<Type, Func<object, IEnumerable>>();

        private IInputElement beforeFocused;

        private bool _IsSearchVisible;
                
        //private bool _contentLoaded;

        public IEnumerable DataSource
        {
            get
            {
                return (IEnumerable)base.GetValue(JFCGrid.DataSourceProperty);
            }
            set
            {
                this.Manager.ResetColumnsWidth();
                base.SetValue(JFCGrid.DataSourceProperty, null);
                base.SetValue(JFCGrid.DataSourceProperty, value);
                this.OnDataSourceChanged();
                this.RefreshScrollBarVertical();
                this.RefreshScrollBarHorizontal();
                this.Manager.RefreshAllColumnsWidth();
                this.Manager.RefreshSizeColumnStar();
                if (this.IsSearchVisible)
                {
                    this.OnSearchChanged(this, new DependencyPropertySearchChangedEventArgs(this.TextBoxSearch.Text, this.SearchResult));
                }
            }
        }

        public IEnumerable DataSourceFooter
        {
            get
            {
                return (IEnumerable)base.GetValue(JFCGrid.DataSourceFooterProperty);
            }
            set
            {
                this.Manager.ResetColumnsWidth();
                base.SetValue(JFCGrid.DataSourceFooterProperty, null);
                base.SetValue(JFCGrid.DataSourceFooterProperty, value);
                this.OnDataSourceFooterChanged();
                this.Manager.RefreshAllColumnsWidth();
            }
        }

        public string PropertyChildrenName
        {
            get
            {
                return (string)base.GetValue(JFCGrid.PropertyChildrenNameProperty);
            }
            set
            {
                base.SetValue(JFCGrid.PropertyChildrenNameProperty, value);
            }
        }

        public string PropertyExpandeChildrenName
        {
            get
            {
                return (string)base.GetValue(JFCGrid.PropertyExpandeChildrenNameProperty);
            }
            set
            {
                base.SetValue(JFCGrid.PropertyExpandeChildrenNameProperty, value);
            }
        }

        internal HashSet<JFCExpendItem> SelectedExpendItems
        {
            get
            {
                return this.selectedExpendItems;
            }
            set
            {
                this.selectedExpendItems = value;
                if (this.selectedExpendItems == null)
                {
                    this.selectedExpendItems = new HashSet<JFCExpendItem>();
                }
                this.Manager.RefreshSelection();
                this.OnSelectedItemsChanged(this, new RoutedEventArgs());
            }
        }

        internal HashSet<JFCExpendItem> SelectedExpendItemsChildren
        {
            get
            {
                return this.selectedExpendItemsChildren;
            }
            set
            {
                this.selectedExpendItemsChildren = value;
                if (this.selectedExpendItemsChildren == null)
                {
                    this.selectedExpendItemsChildren = new HashSet<JFCExpendItem>();
                }
                this.Manager.RefreshSelection();
                this.OnSelectedItemsChanged(this, new RoutedEventArgs());
            }
        }

        public List<object> SelectedItems
        {
            get
            {
                return (from item in this.selectedExpendItems
                        select item.Obj).ToList();
            }
            set
            {
                HashSet<JFCExpendItem> hashSet = new HashSet<JFCExpendItem>();
                if (value != null)
                {
                    foreach (object item in value)
                    {
                        IEnumerable<JFCExpendItem> source = from it in this.dataSource
                                                            where it.Obj == item
                                                            select it;
                        if (source.Count() == 1)
                        {
                            hashSet.Add(source.ElementAt(0));
                        }
                    }
                }
                this.SelectedExpendItems = hashSet;
            }
        }

        public SelectionMode SelectionMode
        {
            get;
            set;
        }

        public int ScrollVerticalValue
        {
            get
            {
                return this.scrollVerticalValue;
            }
            set
            {
                int oldValue = this.scrollVerticalValue;
                this.scrollVerticalValue = value;
                this.ScrollVerticalValueChanged(oldValue, this.scrollVerticalValue);
            }
        }

        public int ScrollHorizontalValue
        {
            get
            {
                return this.scrollHorizontalValue;
            }
            private set
            {
                int oldValue = this.scrollHorizontalValue;
                this.scrollHorizontalValue = value;
                this.ScrollHorizontalValueChanged(oldValue, this.scrollHorizontalValue);
            }
        }

        public int RowHeight
        {
            get
            {
                return (int)base.GetValue(JFCGrid.RowHeightProperty);
            }
            set
            {
                base.SetValue(JFCGrid.RowHeightProperty, value);
            }
        }

        public Brush HorizontalBorderColor
        {
            get
            {
                return (Brush)base.GetValue(JFCGrid.HorizontalBorderColorProperty);
            }
            set
            {
                base.SetValue(JFCGrid.HorizontalBorderColorProperty, value);
            }
        }

        public Brush VerticalBorderColor
        {
            get
            {
                return (Brush)base.GetValue(JFCGrid.VerticalBorderColorProperty);
            }
            set
            {
                base.SetValue(JFCGrid.VerticalBorderColorProperty, value);
            }
        }

        public Brush ScrollViewerBorderColor
        {
            get
            {
                return (Brush)base.GetValue(JFCGrid.ScrollViewerBorderColorProperty);
            }
            set
            {
                base.SetValue(JFCGrid.ScrollViewerBorderColorProperty, value);
            }
        }

        public Thickness ScrollViewerBorderThickness
        {
            get
            {
                return (Thickness)base.GetValue(JFCGrid.ScrollViewerBorderThicknessProperty);
            }
            set
            {
                base.SetValue(JFCGrid.ScrollViewerBorderThicknessProperty, value);
            }
        }

        public Brush ScrollViewerBackgroundColor
        {
            get
            {
                return (Brush)base.GetValue(JFCGrid.ScrollViewerBackgroundColorProperty);
            }
            set
            {
                base.SetValue(JFCGrid.ScrollViewerBackgroundColorProperty, value);
            }
        }

        public Brush ScrollViewerBackgroundColorHeaderColumn
        {
            get
            {
                return (Brush)base.GetValue(JFCGrid.ScrollViewerBackgroundColorHeaderColumnProperty);
            }
            set
            {
                base.SetValue(JFCGrid.ScrollViewerBackgroundColorHeaderColumnProperty, value);
            }
        }

        public ContextMenu ContextmenuRow
        {
            get
            {
                return (ContextMenu)base.GetValue(JFCGrid.ContextmenuRowProperty);
            }
            set
            {
                base.SetValue(JFCGrid.ContextmenuRowProperty, value);
            }
        }

        public bool ColumnMovable
        {
            get
            {
                return (bool)base.GetValue(JFCGrid.ColumnMovableProperty);
            }
            set
            {
                base.SetValue(JFCGrid.ColumnMovableProperty, value);
            }
        }

        public int NbRowMax
        {
            get
            {
                return (int)base.GetValue(JFCGrid.NbRowMaxProperty);
            }
            set
            {
                base.SetValue(JFCGrid.NbRowMaxProperty, value);
            }
        }

        public Visibility ScrollBarVerticalVisibility
        {
            get
            {
                return (Visibility)base.GetValue(JFCGrid.ScrollBarVerticalVisibilityProperty);
            }
            set
            {
                base.SetValue(JFCGrid.ScrollBarVerticalVisibilityProperty, value);
            }
        }

        public Visibility ScrollBarHorizontalVisibility
        {
            get
            {
                return (Visibility)base.GetValue(JFCGrid.ScrollBarHorizontalVisibilityProperty);
            }
            set
            {
                base.SetValue(JFCGrid.ScrollBarHorizontalVisibilityProperty, value);
            }
        }

        public bool IsVirtualised
        {
            get
            {
                return this.isVirtualised;
            }
            set
            {
                this.isVirtualised = value;
                this.RefreshScrollBarHorizontal();
                if (!this.isVirtualised)
                {
                    this.Manager.NbColView = 0;
                    this.Manager.SetStartIndexAndNbCol(0, 2147483647);
                }
            }
        }

        public string LabelResearch
        {
            get
            {
                return (string)base.GetValue(JFCGrid.LabelResearchProperty);
            }
            set
            {
                base.SetValue(JFCGrid.LabelResearchProperty, value);
            }
        }

        public string LabelResult
        {
            get
            {
                return (string)base.GetValue(JFCGrid.LabelResultProperty);
            }
            set
            {
                base.SetValue(JFCGrid.LabelResultProperty, value);
            }
        }

        public string HiliteString
        {
            get
            {
                return (string)base.GetValue(JFCGrid.HiliteStringProperty);
            }
            set
            {
                base.SetValue(JFCGrid.HiliteStringProperty, value);
            }
        }

        public GridLength EcartHeaderColumn
        {
            get
            {
                return (GridLength)base.GetValue(JFCGrid.EcartHeaderColumnProperty);
            }
            set
            {
                base.SetValue(JFCGrid.EcartHeaderColumnProperty, value);
            }
        }

        public GridLength EcartHeaderRow
        {
            get
            {
                return (GridLength)base.GetValue(JFCGrid.EcartHeaderRowProperty);
            }
            set
            {
                base.SetValue(JFCGrid.EcartHeaderRowProperty, value);
            }
        }

        public GridLength EcartBodyFooterHorizontal
        {
            get
            {
                return (GridLength)base.GetValue(JFCGrid.EcartBodyFooterHorizontalProperty);
            }
            set
            {
                base.SetValue(JFCGrid.EcartBodyFooterHorizontalProperty, value);
            }
        }

        public GridLength EcartBodyFooterVertical
        {
            get
            {
                return (GridLength)base.GetValue(JFCGrid.EcartBodyFooterVerticalProperty);
            }
            set
            {
                base.SetValue(JFCGrid.EcartBodyFooterVerticalProperty, value);
            }
        }

        public GridLength EcartScrollHorizontal
        {
            get
            {
                return (GridLength)base.GetValue(JFCGrid.EcartScrollHorizontalProperty);
            }
            set
            {
                base.SetValue(JFCGrid.EcartScrollHorizontalProperty, value);
            }
        }

        public GridLength EcartScrollVertical
        {
            get
            {
                return (GridLength)base.GetValue(JFCGrid.EcartScrollVerticalProperty);
            }
            set
            {
                base.SetValue(JFCGrid.EcartScrollVerticalProperty, value);
            }
        }

        public ContextMenu ContextMenuHeaderColumn
        {
            get
            {
                return this.Grid01.ContextMenu;
            }
            set
            {
                this.Grid00.ContextMenu = value;
                this.Grid01.ContextMenu = value;
                this.Grid02.ContextMenu = value;
            }
        }

        public bool IsPossibleToSearch
        {
            get
            {
                return (bool)base.GetValue(JFCGrid.IsPossibleToSearchProperty);
            }
            set
            {
                base.SetValue(JFCGrid.IsPossibleToSearchProperty, value);
            }
        }

        public int NbRowDisplayed
        {
            get
            {
                int num = this.NbRowAffichableEntiere();
                if (this.Grid11.RowDefinitions.Count() < num)
                {
                    return this.Grid11.RowDefinitions.Count();
                }
                return num;
            }
        }

        public bool IsSearchVisible
        {
            get
            {
                return this._IsSearchVisible;
            }
            set
            {
                this._IsSearchVisible = value;
                if (value)
                {
                    this.RowSearch.Height = default(GridLength);
                    this.beforeFocused = Keyboard.FocusedElement;
                    this.WrapPanelSearch.Visibility = Visibility.Visible;
                    this.TextBoxSearch.Focus();
                    this.Manager.RowRefreshDataAll();
                }
                else
                {
                    this.RowSearch.Height = new GridLength(0.0);
                    if (this.beforeFocused != null)
                    {
                        this.beforeFocused.Focus();
                    }
                    this.WrapPanelSearch.Visibility = Visibility.Collapsed;
                    this.Manager.RowRefreshDataAll();
                }
            }
        }

        public event RoutedEventHandler ColumnMoved;

        public event RoutedEventHandler SelectedItemsChanged;

        public event DependencyPropertyLineOverChangedEventHandler LineOverChanged;

        private event DependencyPropertySearchChangedEventHandler SearchChangedInternal;

        public event DependencyPropertySearchChangedEventHandler SearchChanged
        {
            add
            {
                this.SearchChangedInternal += value;
                this.IsPossibleToSearch = true;
            }
            remove
            {
                this.SearchChangedInternal -= value;
                this.IsPossibleToSearch = false;
            }
        }

        internal void OnColumnMoved(object sender, RoutedEventArgs e)
        {
            if (this.ColumnMoved != null)
            {
                this.ColumnMoved(this, e);
            }
        }

        internal void OnSelectedItemsChanged(object sender, RoutedEventArgs e)
        {
            if (this.SelectedItemsChanged != null)
            {
                this.SelectedItemsChanged(this, e);
            }
        }

        internal void OnLineOverChanged(object sender, DependencyPropertyLineOverChangedEventArgs e)
        {
            if (this.LineOverChanged != null)
            {
                this.LineOverChanged(this, e);
            }
        }

        internal void OnSearchChanged(object sender, DependencyPropertySearchChangedEventArgs e)
        {
            if (this.SearchChangedInternal != null)
            {
                this.SearchResult.Clear();
                this.SearchChangedInternal(this, e);
            }
        }

        public JFCGrid()
        {
            this.InitializeComponent();
            this.Manager = new JFCGridManager(this);
            this.ColunmDefinition.CollectionChanged += this.ColunmDefinition_CollectionChanged;
            this.ColunmDefinitionFooter.CollectionChanged += this.ColunmDefinitionFooter_CollectionChanged;
            this.SearchResult.CollectionChanged += this.SearchResult_CollectionChanged;
            base.PreviewKeyDown += this.JFCGrid_PreviewKeyDown;
        }

        private void JFCGrid_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Up)
            {
                this.ScrollBarVertical.Value = this.ScrollBarVertical.Value - 1.0;
            }
            else if (e.Key == Key.Down)
            {
                this.ScrollBarVertical.Value = this.ScrollBarVertical.Value + 1.0;
            }
            else if (e.Key == Key.Prior)
            {
                this.ScrollBarVertical.Value = this.ScrollBarVertical.Value - this.ScrollBarVertical.ViewportSize;
            }
            else if (e.Key == Key.Next)
            {
                this.ScrollBarVertical.Value = this.ScrollBarVertical.Value + this.ScrollBarVertical.ViewportSize;
            }
        }

        private void SearchResult_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                if (this.SearchResult.Count() > 0)
                {
                    base.Dispatcher.BeginInvoke(DispatcherPriority.Normal, (Action)delegate
                    {
                        this.IndexSearch = 0;
                        GridPosition gridPosition = ((Collection<GridPosition>)this.SearchResult)[this.IndexSearch];
                        this.FlagSearchObject(this.dataSource);
                        this.ScrollVerticalTo(gridPosition.Path);
                        TextBlock textBlockNbFind3 = this.TextBlockNbFind;
                        int num3 = this.IndexSearch + 1;
                        string str3 = num3.ToString();
                        num3 = this.SearchResult.Count();
                        textBlockNbFind3.Text = str3 + " / " + num3.ToString();
                    });
                }
                else
                {
                    this.IndexSearch = -1;
                }
            }
            else if (e.Action != NotifyCollectionChangedAction.Move)
            {
                if (e.Action == NotifyCollectionChangedAction.Remove)
                {
                    if (this.SearchResult.Count() > 0)
                    {
                        base.Dispatcher.BeginInvoke(DispatcherPriority.Normal, (Action)delegate
                        {
                            this.IndexSearch = 0;
                            this.ScrollVerticalTo(((Collection<GridPosition>)this.SearchResult)[this.IndexSearch].Path);
                            TextBlock textBlockNbFind2 = this.TextBlockNbFind;
                            int num2 = this.IndexSearch + 1;
                            string str2 = num2.ToString();
                            num2 = this.SearchResult.Count();
                            textBlockNbFind2.Text = str2 + " / " + num2.ToString();
                        });
                    }
                    else
                    {
                        base.Dispatcher.BeginInvoke(DispatcherPriority.Normal, (Action)delegate
                        {
                            this.IndexSearch = -1;
                            this.Manager.RowRefreshDataAll();
                        });
                    }
                }
                else if (e.Action != NotifyCollectionChangedAction.Replace && e.Action == NotifyCollectionChangedAction.Reset)
                {
                    this.IndexSearch = -1;
                    base.Dispatcher.BeginInvoke(DispatcherPriority.Normal, (Action)delegate
                    {
                        this.Manager.RowRefreshDataAll();
                    });
                }
            }
            if (this.IndexSearch == -1)
            {
                this.TextBlockNbFind.Text = "0 / 0";
            }
            else
            {
                TextBlock textBlockNbFind = this.TextBlockNbFind;
                int num = this.IndexSearch + 1;
                string str = num.ToString();
                num = this.SearchResult.Count();
                textBlockNbFind.Text = str + " / " + num.ToString();
            }
        }

        private void ParamColumn(JFCGridColumn col)
        {
            if (col.TypeColumn == JFCGridColumn.TypesColumn.GroupingNoRow)
            {
                if (col.LevelGrouping != -1 && !this.LstGroupingNoRowLevel.Contains(col.LevelGrouping))
                {
                    this.LstGroupingNoRowLevel.Add(col.LevelGrouping);
                }
            }
            else if (col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRow)
            {
                if (col.LevelGrouping != -1 && !this.LstGroupingWithRowLevel.Contains(col.LevelGrouping))
                {
                    this.LstGroupingWithRowLevel.Add(col.LevelGrouping);
                }
            }
            else if (col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRowWhenNoChildren && col.LevelGrouping != -1)
            {
                if (!this.LstGroupingNoRowLevel.Contains(col.LevelGrouping))
                {
                    this.LstGroupingNoRowLevel.Add(col.LevelGrouping);
                }
                if (!this.LstGroupingWithRowWhenNoChildrenLevel.Contains(col.LevelGrouping))
                {
                    this.LstGroupingWithRowWhenNoChildrenLevel.Add(col.LevelGrouping);
                }
            }
        }

        private void ColunmDefinition_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            JFCGridControl.ObservableCollection<JFCGridColumn> observableCollection = sender as JFCGridControl.ObservableCollection<JFCGridColumn>;
            bool flag = false;
            bool flag2 = false;
            bool flag3 = false;
            List<JFCGridColumn>.Enumerator enumerator2;
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                if (observableCollection.Parent == null)
                {
                    foreach (JFCGridColumn newItem in e.NewItems)
                    {
                        this.Manager.BuildParentColumn(newItem);
                        this.AddEvent(newItem);
                        if (newItem.ChildrenColumns.Count() != 0)
                        {
                            enumerator2 = this.Manager.FindChildrenColumn(newItem.ChildrenColumns).GetEnumerator();
                            try
                            {
                                while (enumerator2.MoveNext())
                                {
                                    JFCGridColumn current = enumerator2.Current;
                                    this.Manager.ColumnAdd(current);
                                    this.ParamColumn(current);
                                    if (current.Frozen == JFCGridColumn.FrozenType.Start)
                                    {
                                        flag = true;
                                    }
                                    else if (current.Frozen == JFCGridColumn.FrozenType.End)
                                    {
                                        flag2 = true;
                                    }
                                    else
                                    {
                                        flag3 = true;
                                    }
                                }
                            }
                            finally
                            {
                                ((IDisposable)enumerator2).Dispose();
                            }
                        }
                        else
                        {
                            this.Manager.ColumnAdd(newItem);
                            this.ParamColumn(newItem);
                            if (newItem.Frozen == JFCGridColumn.FrozenType.Start)
                            {
                                flag = true;
                            }
                            else if (newItem.Frozen == JFCGridColumn.FrozenType.End)
                            {
                                flag2 = true;
                            }
                            else
                            {
                                flag3 = true;
                            }
                        }
                    }
                }
                else if (observableCollection.Parent is JFCGridColumn)
                {
                    JFCGridColumn column2 = observableCollection.Parent as JFCGridColumn;
                    int num = 0;
                    if (observableCollection.Count > e.NewItems.Count)
                    {
                        int num2 = observableCollection.Count - e.NewItems.Count;
                        JFCGridColumn jFCGridColumn2 = ((Collection<JFCGridColumn>)observableCollection)[num2 - 1];
                        if (jFCGridColumn2.ChildrenColumns.Count == 0)
                        {
                            num = this.Manager.ColumnIndexOf(jFCGridColumn2);
                            num++;
                        }
                        else
                        {
                            JFCGridColumn column3 = this.Manager.FindLastChildrenColumn(jFCGridColumn2.ChildrenColumns);
                            num = this.Manager.ColumnIndexOf(column3);
                            num++;
                        }
                    }
                    else
                    {
                        num = this.Manager.ColumnIndexOf(column2);
                        this.Manager.ColumnRemove(column2);
                        this.Manager.NbColView = 0;
                    }
                    if (num >= 0)
                    {
                        int num3 = 0;
                        foreach (JFCGridColumn newItem2 in e.NewItems)
                        {
                            this.Manager.BuildParentColumn(newItem2);
                            this.AddEvent(newItem2);
                            if (newItem2.ChildrenColumns.Count() != 0)
                            {
                                enumerator2 = this.Manager.FindChildrenColumn(newItem2.ChildrenColumns).GetEnumerator();
                                try
                                {
                                    while (enumerator2.MoveNext())
                                    {
                                        JFCGridColumn current2 = enumerator2.Current;
                                        this.Manager.ColumnAdd(current2, num + num3);
                                        this.ParamColumn(current2);
                                        if (current2.Frozen == JFCGridColumn.FrozenType.Start)
                                        {
                                            flag = true;
                                        }
                                        else if (current2.Frozen == JFCGridColumn.FrozenType.End)
                                        {
                                            flag2 = true;
                                        }
                                        else
                                        {
                                            flag3 = true;
                                        }
                                        num3++;
                                    }
                                }
                                finally
                                {
                                    ((IDisposable)enumerator2).Dispose();
                                }
                            }
                            else
                            {
                                this.Manager.ColumnAdd(newItem2, num + num3);
                                this.ParamColumn(newItem2);
                                if (newItem2.Frozen == JFCGridColumn.FrozenType.Start)
                                {
                                    flag = true;
                                }
                                else if (newItem2.Frozen == JFCGridColumn.FrozenType.End)
                                {
                                    flag2 = true;
                                }
                                else
                                {
                                    flag3 = true;
                                }
                                num3++;
                            }
                        }
                    }
                }
            }
            else
            {
                if (e.Action == NotifyCollectionChangedAction.Move)
                {
                    if (e.NewStartingIndex != e.OldStartingIndex)
                    {
                        ((JFCGridControl.ObservableCollection<JFCGridColumn>)sender).Count();
                        foreach (JFCGridColumn oldItem in e.OldItems)
                        {
                            this.Manager.BuildParentColumn(oldItem);
                            JFCGridControl.ObservableCollection<JFCGridColumn> source = (oldItem.Parent != null) ? oldItem.Parent.ChildrenColumns : this.ColunmDefinition;
                            List<JFCGridColumn> list = (from column in source
                                                        where column.Frozen == oldItem.Frozen
                                                        select column).ToList();
                            int num4 = list.IndexOf(oldItem);
                            if (e.NewStartingIndex < e.OldStartingIndex)
                            {
                                int num5;
                                if (num4 + 1 < list.Count())
                                {
                                    if (list[num4 + 1].ChildrenColumns.Count() > 0)
                                    {
                                        List<JFCGridColumn> list2 = this.Manager.FindChildrenColumn(list[num4 + 1].ChildrenColumns);
                                        num5 = this.Manager.ColumnIndexOf(list2[0]);
                                    }
                                    else
                                    {
                                        num5 = this.Manager.ColumnIndexOf(list[num4 + 1]);
                                    }
                                }
                                else
                                {
                                    num5 = this.Manager.NbColumn(oldItem.Frozen) - 1;
                                }
                                if (oldItem.ChildrenColumns.Count() > 0)
                                {
                                    List<JFCGridColumn> list3 = this.Manager.FindChildrenColumn(oldItem.ChildrenColumns);
                                    int num6 = 0;
                                    enumerator2 = list3.GetEnumerator();
                                    try
                                    {
                                        while (enumerator2.MoveNext())
                                        {
                                            JFCGridColumn current3 = enumerator2.Current;
                                            this.Manager.ColumnMove(current3, num5 + num6);
                                            if (current3.Frozen == JFCGridColumn.FrozenType.Start)
                                            {
                                                flag = true;
                                            }
                                            else if (current3.Frozen == JFCGridColumn.FrozenType.End)
                                            {
                                                flag2 = true;
                                            }
                                            else
                                            {
                                                flag3 = true;
                                            }
                                            num6++;
                                        }
                                    }
                                    finally
                                    {
                                        ((IDisposable)enumerator2).Dispose();
                                    }
                                }
                                else
                                {
                                    this.Manager.ColumnMove(oldItem, num5);
                                    if (oldItem.Frozen == JFCGridColumn.FrozenType.Start)
                                    {
                                        flag = true;
                                    }
                                    else if (oldItem.Frozen == JFCGridColumn.FrozenType.End)
                                    {
                                        flag2 = true;
                                    }
                                    else
                                    {
                                        flag3 = true;
                                    }
                                }
                            }
                            else if (e.NewStartingIndex > e.OldStartingIndex)
                            {
                                int num7 = 0;
                                int num5;
                                if (oldItem.ChildrenColumns.Count() > 0)
                                {
                                    if (num4 + 1 < list.Count())
                                    {
                                        if (list[num4 + 1].ChildrenColumns.Count() > 0)
                                        {
                                            List<JFCGridColumn> list2 = this.Manager.FindChildrenColumn(list[num4 + 1].ChildrenColumns);
                                            num5 = this.Manager.ColumnIndexOf(list2[0]);
                                        }
                                        else
                                        {
                                            num5 = this.Manager.ColumnIndexOf(list[num4 + 1]);
                                        }
                                        num5--;
                                    }
                                    else if (list[num4 - 1].ChildrenColumns.Count() > 0)
                                    {
                                        List<JFCGridColumn> list2 = this.Manager.FindChildrenColumn(list[num4 - 1].ChildrenColumns);
                                        num5 = this.Manager.ColumnIndexOf(list2[list2.Count() - 1]);
                                    }
                                    else
                                    {
                                        num5 = this.Manager.ColumnIndexOf(list[num4 - 1]);
                                    }
                                }
                                else if (list[num4 - 1].ChildrenColumns.Count() == 0)
                                {
                                    num5 = this.Manager.ColumnIndexOf(list[num4 - 1]);
                                }
                                else
                                {
                                    List<JFCGridColumn> list2 = this.Manager.FindChildrenColumn(list[num4 - 1].ChildrenColumns);
                                    num5 = this.Manager.ColumnIndexOf(list2[list2.Count() - 1]);
                                }
                                if (oldItem.ChildrenColumns.Count() > 0)
                                {
                                    enumerator2 = this.Manager.FindChildrenColumn(oldItem.ChildrenColumns).GetEnumerator();
                                    try
                                    {
                                        while (enumerator2.MoveNext())
                                        {
                                            JFCGridColumn current4 = enumerator2.Current;
                                            this.Manager.ColumnMove(current4, num5);
                                            if (current4.Frozen == JFCGridColumn.FrozenType.Start)
                                            {
                                                flag = true;
                                            }
                                            else if (current4.Frozen == JFCGridColumn.FrozenType.End)
                                            {
                                                flag2 = true;
                                            }
                                            else
                                            {
                                                flag3 = true;
                                            }
                                            num7++;
                                        }
                                    }
                                    finally
                                    {
                                        ((IDisposable)enumerator2).Dispose();
                                    }
                                }
                                else
                                {
                                    this.Manager.ColumnMove(oldItem, num5);
                                    if (oldItem.Frozen == JFCGridColumn.FrozenType.Start)
                                    {
                                        flag = true;
                                    }
                                    else if (oldItem.Frozen == JFCGridColumn.FrozenType.End)
                                    {
                                        flag2 = true;
                                    }
                                    else
                                    {
                                        flag3 = true;
                                    }
                                }
                            }
                        }
                        this.OnColumnMoved(this, new RoutedEventArgs());
                        goto IL_0a03;
                    }
                    return;
                }
                if (e.Action == NotifyCollectionChangedAction.Remove)
                {
                    int index = 0;
                    foreach (JFCGridColumn oldItem2 in e.OldItems)
                    {
                        this.RemoveEvent(oldItem2.ChildrenColumns);
                        if (oldItem2.ChildrenColumns.Count() != 0)
                        {
                            enumerator2 = this.Manager.FindChildrenColumn(oldItem2.ChildrenColumns).GetEnumerator();
                            try
                            {
                                while (enumerator2.MoveNext())
                                {
                                    JFCGridColumn current5 = enumerator2.Current;
                                    index = this.Manager.ColumnIndexOf(current5);
                                    this.Manager.ColumnRemove(current5);
                                    current5.ChildrenColumns.CollectionChanged -= this.ColunmDefinition_CollectionChanged;
                                    this.ParamColumn(current5);
                                    if (current5.Frozen == JFCGridColumn.FrozenType.Start)
                                    {
                                        flag = true;
                                    }
                                    else if (current5.Frozen == JFCGridColumn.FrozenType.End)
                                    {
                                        flag2 = true;
                                    }
                                    else
                                    {
                                        flag3 = true;
                                    }
                                }
                            }
                            finally
                            {
                                ((IDisposable)enumerator2).Dispose();
                            }
                        }
                        else
                        {
                            index = this.Manager.ColumnIndexOf(oldItem2);
                            this.Manager.ColumnRemove(oldItem2);
                            oldItem2.ChildrenColumns.CollectionChanged -= this.ColunmDefinition_CollectionChanged;
                            this.ParamColumn(oldItem2);
                            if (oldItem2.Frozen == JFCGridColumn.FrozenType.Start)
                            {
                                flag = true;
                            }
                            else if (oldItem2.Frozen == JFCGridColumn.FrozenType.End)
                            {
                                flag2 = true;
                            }
                            else
                            {
                                flag3 = true;
                            }
                        }
                    }
                    if (observableCollection.Count == 0)
                    {
                        JFCGridColumn column4 = observableCollection.Parent as JFCGridColumn;
                        if (observableCollection.Parent != null)
                        {
                            this.Manager.NbColView = 0;
                            this.Manager.ColumnAdd(column4, index);
                        }
                    }
                }
                else if (e.Action == NotifyCollectionChangedAction.Replace)
                {
                    new Exception("Replace column is not implemented");
                }
                else if (e.Action == NotifyCollectionChangedAction.Reset)
                {
                    if (observableCollection.Parent == null)
                    {
                        this.Manager.ColumnRemoveAll();
                        this.LstGroupingNoRowLevel.Clear();
                        this.LstGroupingWithRowLevel.Clear();
                        this.LstGroupingWithRowWhenNoChildrenLevel.Clear();
                        flag = true;
                        flag3 = true;
                    }
                    else if (((JFCGridColumn)observableCollection.Parent).Frozen == JFCGridColumn.FrozenType.Start)
                    {
                        flag = true;
                    }
                    else if (((JFCGridColumn)observableCollection.Parent).Frozen == JFCGridColumn.FrozenType.End)
                    {
                        flag2 = true;
                    }
                    else
                    {
                        flag3 = true;
                    }
                }
            }
            goto IL_0a03;
        IL_0a03:
            this.Manager.RefreshSizeColumnStar();
            if (flag)
            {
                this.Manager.ReBuildColumnHeader(JFCGridColumn.FrozenType.Start);
            }
            if (flag2)
            {
                this.Manager.ReBuildColumnHeader(JFCGridColumn.FrozenType.End);
            }
            if (flag3)
            {
                this.RefreshScrollBarHorizontal();
            }
            JFCGrid.UpdateEcart(this, default(DependencyPropertyChangedEventArgs));
        }

        private void ColunmDefinitionFooter_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            List<JFCGridColumn>.Enumerator enumerator2;
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                foreach (JFCGridColumn newItem in e.NewItems)
                {
                    this.Manager.BuildParentColumn(newItem);
                    this.AddEventFooter(newItem);
                    if (newItem.ChildrenColumns.Count() != 0)
                    {
                        enumerator2 = this.Manager.FindChildrenColumn(newItem.ChildrenColumns).GetEnumerator();
                        try
                        {
                            while (enumerator2.MoveNext())
                            {
                                JFCGridColumn current = enumerator2.Current;
                                this.Manager.ColumnAddHeaderFooter(current);
                            }
                        }
                        finally
                        {
                            ((IDisposable)enumerator2).Dispose();
                        }
                    }
                    else
                    {
                        this.Manager.ColumnAddHeaderFooter(newItem);
                    }
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Move)
            {
                if (e.NewStartingIndex != e.OldStartingIndex)
                {
                    ((JFCGridControl.ObservableCollection<JFCGridColumn>)sender).Count();
                    foreach (JFCGridColumn oldItem in e.OldItems)
                    {
                        this.Manager.BuildParentColumn(oldItem);
                        JFCGridControl.ObservableCollection<JFCGridColumn> source = (oldItem.Parent != null) ? oldItem.Parent.ChildrenColumns : this.ColunmDefinitionFooter;
                        List<JFCGridColumn> list = (from column in source
                                                    where column.Frozen == oldItem.Frozen
                                                    select column).ToList();
                        int num = list.IndexOf(oldItem);
                        if (e.NewStartingIndex < e.OldStartingIndex)
                        {
                            int num2;
                            if (list.Count() < num + 1)
                            {
                                if (list[num + 1].ChildrenColumns.Count() > 0)
                                {
                                    List<JFCGridColumn> list2 = this.Manager.FindChildrenColumn(list[num + 1].ChildrenColumns);
                                    num2 = this.Manager.ColumnHeaderRowFooterIndexOf(list2[0]);
                                }
                                else
                                {
                                    num2 = this.Manager.ColumnHeaderRowFooterIndexOf(list[num + 1]);
                                }
                            }
                            else
                            {
                                num2 = this.Manager.NbColumnHeaderRowFooter() - 1;
                            }
                            if (oldItem.ChildrenColumns.Count() > 0)
                            {
                                List<JFCGridColumn> list3 = this.Manager.FindChildrenColumn(oldItem.ChildrenColumns);
                                int num3 = 0;
                                enumerator2 = list3.GetEnumerator();
                                try
                                {
                                    while (enumerator2.MoveNext())
                                    {
                                        JFCGridColumn current2 = enumerator2.Current;
                                        this.Manager.ColumnMove(current2, num2 + num3);
                                        num3++;
                                    }
                                }
                                finally
                                {
                                    ((IDisposable)enumerator2).Dispose();
                                }
                            }
                            else
                            {
                                this.Manager.ColumnMove(oldItem, num2);
                            }
                        }
                        else if (e.NewStartingIndex > e.OldStartingIndex)
                        {
                            int num4 = 0;
                            int num2;
                            if (oldItem.ChildrenColumns.Count() > 0)
                            {
                                if (num + 1 < list.Count())
                                {
                                    if (list[num + 1].ChildrenColumns.Count() > 0)
                                    {
                                        List<JFCGridColumn> list2 = this.Manager.FindChildrenColumn(list[num + 1].ChildrenColumns);
                                        num2 = this.Manager.ColumnIndexOf(list2[0]);
                                    }
                                    else
                                    {
                                        num2 = this.Manager.ColumnIndexOf(list[num + 1]);
                                    }
                                    num2--;
                                }
                                else if (list[num - 1].ChildrenColumns.Count() > 0)
                                {
                                    List<JFCGridColumn> list2 = this.Manager.FindChildrenColumn(list[num - 1].ChildrenColumns);
                                    num2 = this.Manager.ColumnIndexOf(list2[list2.Count() - 1]);
                                }
                                else
                                {
                                    num2 = this.Manager.ColumnIndexOf(list[num - 1]);
                                }
                            }
                            else if (list[num - 1].ChildrenColumns.Count() == 0)
                            {
                                num2 = this.Manager.ColumnIndexOf(list[num - 1]);
                            }
                            else
                            {
                                List<JFCGridColumn> list2 = this.Manager.FindChildrenColumn(list[num - 1].ChildrenColumns);
                                num2 = this.Manager.ColumnIndexOf(list2[list2.Count() - 1]);
                            }
                            if (oldItem.ChildrenColumns.Count() > 0)
                            {
                                enumerator2 = this.Manager.FindChildrenColumn(oldItem.ChildrenColumns).GetEnumerator();
                                try
                                {
                                    while (enumerator2.MoveNext())
                                    {
                                        JFCGridColumn current3 = enumerator2.Current;
                                        this.Manager.ColumnMove(current3, num2);
                                        num4++;
                                    }
                                }
                                finally
                                {
                                    ((IDisposable)enumerator2).Dispose();
                                }
                            }
                            else
                            {
                                this.Manager.ColumnMove(oldItem, num2);
                            }
                        }
                    }
                    this.OnColumnMoved(this, new RoutedEventArgs());
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove)
            {
                bool flag = false;
                bool flag2 = false;
                bool flag3 = false;
                foreach (JFCGridColumn oldItem2 in e.OldItems)
                {
                    this.Manager.ColumnRemove(oldItem2);
                    this.RemoveEventFooter(oldItem2.ChildrenColumns);
                    if (oldItem2.Frozen == JFCGridColumn.FrozenType.Start)
                    {
                        flag = true;
                    }
                    if (oldItem2.Frozen == JFCGridColumn.FrozenType.End)
                    {
                        flag2 = true;
                    }
                    else
                    {
                        flag3 = true;
                    }
                }
                if (flag)
                {
                    this.Manager.BuildHeader(this.Grid00);
                }
                if (flag2)
                {
                    this.Manager.BuildHeader(this.Grid02);
                }
                if (flag3)
                {
                    this.Manager.BuildHeader(this.Grid01);
                }
            }
            else if (e.Action != NotifyCollectionChangedAction.Replace && e.Action == NotifyCollectionChangedAction.Reset)
            {
                this.Manager.ColumnRemoveAll();
            }
        }

        private void AddEvent(JFCGridColumn Col)
        {
            Col.ChildrenColumns.CollectionChanged += this.ColunmDefinition_CollectionChanged;
            Col.Header.ContentChanged += this.Header_ContentChanged;
            foreach (JFCGridColumn childrenColumn in Col.ChildrenColumns)
            {
                this.AddEvent(childrenColumn);
            }
        }

        private void RemoveEvent(JFCGridControl.ObservableCollection<JFCGridColumn> ListCol)
        {
            ListCol.CollectionChanged -= this.ColunmDefinition_CollectionChanged;
            foreach (JFCGridColumn item in ListCol)
            {
                item.Header.ContentChanged -= this.Header_ContentChanged;
                this.RemoveEvent(item.ChildrenColumns);
            }
        }

        private void AddEventFooter(JFCGridColumn Col)
        {
            Col.ChildrenColumns.CollectionChanged += this.ColunmDefinition_CollectionChanged;
            Col.Header.ContentChanged += this.Header_ContentChanged;
            foreach (JFCGridColumn childrenColumn in Col.ChildrenColumns)
            {
                this.AddEventFooter(childrenColumn);
            }
        }

        private void RemoveEventFooter(JFCGridControl.ObservableCollection<JFCGridColumn> ListCol)
        {
            ListCol.CollectionChanged -= this.ColunmDefinition_CollectionChanged;
            foreach (JFCGridColumn item in ListCol)
            {
                item.Header.ContentChanged -= this.Header_ContentChanged;
                this.RemoveEventFooter(item.ChildrenColumns);
            }
        }

        internal void Header_ContentChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            JFCGridColumnHeader jFCGridColumnHeader = sender as JFCGridColumnHeader;
            if (jFCGridColumnHeader.Content != null)
            {
                jFCGridColumnHeader.Column.ActualWidthHeader = this.Manager.CalculTailleHeaderMaxi(jFCGridColumnHeader.Column);
            }
            else
            {
                jFCGridColumnHeader.Column.ActualWidthHeader = default(GridLength);
            }
        }

        internal static void UpdateDatasource(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid jFCGrid = obj as JFCGrid;
            if (jFCGrid.DataSource != null)
            {
                List<JFCExpendItem> list = new List<JFCExpendItem>();
                int num = 0;
                int num2 = 0;
                foreach (object item in jFCGrid.DataSource)
                {
                    JFCExpendItem jFCExpendItem = new JFCExpendItem(item);
                    if (!jFCGrid.LstGroupingNoRowLevel.Contains(jFCExpendItem.Level))
                    {
                        list.Add(jFCExpendItem);
                        jFCExpendItem.StartChildViewIndex = num;
                        num++;
                        if (jFCGrid.PropertyChildrenName != "")
                        {
                            num2 = jFCGrid.t(list, jFCExpendItem, num);
                            jFCExpendItem.NbChildrenView = num2 + 1;
                            num += num2;
                        }
                    }
                    else if (jFCGrid.PropertyChildrenName != "")
                    {
                        jFCExpendItem.StartChildViewIndex = num;
                        jFCExpendItem.NbChildrenView = jFCGrid.t(list, jFCExpendItem, num);
                        num += jFCExpendItem.NbChildrenView;
                        if (jFCExpendItem.NbChildrenView == 0 && jFCGrid.LstGroupingWithRowWhenNoChildrenLevel.Contains(jFCExpendItem.Level))
                        {
                            list.Add(jFCExpendItem);
                            num++;
                        }
                    }
                }
                jFCGrid.dataSource = list.ToArray();
            }
            else if (jFCGrid.dataSource != null)
            {
                JFCExpendItem[] array = jFCGrid.dataSource;
                for (int i = 0; i < array.Length; i++)
                {
                    array[i].ResetEvent();
                }
                jFCGrid.dataSource = null;
            }
        }

        private int t(List<JFCExpendItem> lstAdd, JFCExpendItem item, int idxItem)
        {
            if (item.Obj == null)
            {
                return 0;
            }
            Type type = item.Obj.GetType();
            Func<object, IEnumerable> func = default(Func<object, IEnumerable>);
            if (!JFCGrid.propGetter.TryGetValue(type, out func))
            {
                ParameterExpression parameterExpression = System.Linq.Expressions.Expression.Parameter(typeof(object), "itemobj");
                func = System.Linq.Expressions.Expression.Lambda<Func<object, IEnumerable>>((System.Linq.Expressions.Expression)System.Linq.Expressions.Expression.Convert(System.Linq.Expressions.Expression.Property(System.Linq.Expressions.Expression.Convert(parameterExpression, type), this.PropertyChildrenName), typeof(IEnumerable)), new ParameterExpression[1]
                {
                parameterExpression
                }).Compile();
                JFCGrid.propGetter.Add(type, func);
            }
            if (func != null)
            {
                IEnumerable enumerable = func(item.Obj);
                if (enumerable != null && enumerable.GetEnumerator().MoveNext())
                {
                    item.IsExpendable = true;
                }
                if (item.IsExpendable)
                {
                    if (this.PropertyExpandeChildrenName != null && this.PropertyExpandeChildrenName != "")
                    {
                        int result = 0;
                        PropertyInfo property = item.Obj.GetType().GetProperty(this.PropertyExpandeChildrenName);
                        if (property != (PropertyInfo)null)
                        {
                            if (property.PropertyType == typeof(bool))
                            {
                                bool flag = (bool)property.GetValue(item.Obj, null);
                                if (flag && item.IsExpendable)
                                {
                                    item.IsExpended = flag;
                                    result = this.ExpandeItem(lstAdd, item, idxItem, enumerable);
                                }
                            }
                            item.PropertyChanged -= this.item_PropertyChanged;
                            item.PropertyChanged += this.item_PropertyChanged;
                            Binding binding = new Binding(this.PropertyExpandeChildrenName);
                            binding.Mode = BindingMode.TwoWay;
                            binding.Source = item.Obj;
                            BindingOperations.SetBinding(item, JFCExpendItem.IsExpendedProperty, binding);
                            return result;
                        }
                    }
                    else
                    {
                        item.PropertyChanged -= this.item_PropertyChanged;
                        item.PropertyChanged += this.item_PropertyChanged;
                    }
                }
            }
            return 0;
        }

        private int t2(List<JFCExpendItem> lstDS, JFCExpendItem item, int idxItem)
        {
            if (item.Obj == null)
            {
                return 0;
            }
            Type type = item.Obj.GetType();
            Func<object, IEnumerable> func = default(Func<object, IEnumerable>);
            if (!JFCGrid.propGetter.TryGetValue(type, out func))
            {
                ParameterExpression parameterExpression = System.Linq.Expressions.Expression.Parameter(typeof(object), "itemobj");
                func = System.Linq.Expressions.Expression.Lambda<Func<object, IEnumerable>>((System.Linq.Expressions.Expression)System.Linq.Expressions.Expression.Convert(System.Linq.Expressions.Expression.Property(System.Linq.Expressions.Expression.Convert(parameterExpression, type), this.PropertyChildrenName), typeof(IEnumerable)), new ParameterExpression[1]
                {
                parameterExpression
                }).Compile();
                JFCGrid.propGetter.Add(type, func);
            }
            if (func != null)
            {
                IEnumerable enumerable = func(item.Obj);
                if (enumerable != null && enumerable.GetEnumerator().MoveNext())
                {
                    item.IsExpendable = true;
                }
                if (item.IsExpendable && this.PropertyExpandeChildrenName != null && this.PropertyExpandeChildrenName != "")
                {
                    int result = 0;
                    PropertyInfo property = item.Obj.GetType().GetProperty(this.PropertyExpandeChildrenName);
                    if (property != (PropertyInfo)null)
                    {
                        if (property.PropertyType == typeof(bool))
                        {
                            bool flag = (bool)property.GetValue(item.Obj, null);
                            if (flag && item.IsExpendable)
                            {
                                item.IsExpended = flag;
                                result = this.ExpandeItem2(lstDS, item, idxItem, enumerable);
                            }
                        }
                        return result;
                    }
                }
            }
            return 0;
        }

        private void UpdateNbChildren(JFCExpendItem item, int nbChildren)
        {
            if (item.Parent != null)
            {
                item.Parent.NbChildrenView += nbChildren;
                this.UpdateNbChildren(item.Parent, nbChildren);
            }
        }

        private void item_PropertyChanged(object sender, PropertyChangedExtendedEventArgs<object> e)
        {
            if (e.PropertyName == "IsExpended" && e.OldValue != e.NewValue)
            {
                JFCExpendItem jFCExpendItem = sender as JFCExpendItem;
                PropertyInfo property = jFCExpendItem.Obj.GetType().GetProperty(this.PropertyChildrenName);
                List<JFCExpendItem> list = this.dataSource.ToList();
                int num = list.IndexOf(jFCExpendItem);
                if (jFCExpendItem.IsExpended)
                {
                    num++;
                }
                if (jFCExpendItem.IsExpended)
                {
                    List<JFCExpendItem> list2 = new List<JFCExpendItem>();
                    this.ExpandeItem(list2, jFCExpendItem, num, property);
                    list.InsertRange(num, list2);
                }
                else
                {
                    this.ExpandeItem(list, jFCExpendItem, num, property);
                }
                if (this.LstGroupingNoRowLevel.Count() != 0 || this.LstGroupingWithRowLevel.Count() != 0)
                {
                    JFCExpendItem jFCExpendItem2 = list.First();
                    while (jFCExpendItem2.Parent != null)
                    {
                        jFCExpendItem2 = jFCExpendItem2.Parent;
                    }
                    int num2 = 0;
                    num = 0;
                    foreach (object item in this.DataSource)
                    {
                        JFCExpendItem jFCExpendItem3 = null;
                        if (list[num].Obj == item)
                        {
                            jFCExpendItem3 = list[num];
                        }
                        else
                        {
                            JFCExpendItem jFCExpendItem4 = list[num];
                            while (jFCExpendItem4.Obj != item)
                            {
                                jFCExpendItem4 = jFCExpendItem4.Parent;
                                if (jFCExpendItem4 == null)
                                {
                                    break;
                                }
                            }
                            jFCExpendItem3 = ((jFCExpendItem4 != null) ? ((jFCExpendItem4.Obj != item) ? null : jFCExpendItem4) : null);
                        }
                        if (!this.LstGroupingNoRowLevel.Contains(jFCExpendItem3.Level))
                        {
                            jFCExpendItem3.StartChildViewIndex = num;
                            num++;
                            if (this.PropertyChildrenName != "")
                            {
                                num2 = this.t2(list, jFCExpendItem3, num);
                                jFCExpendItem3.NbChildrenView = num2 + 1;
                                num += num2;
                            }
                        }
                        else if (this.PropertyChildrenName != "")
                        {
                            jFCExpendItem3.StartChildViewIndex = num;
                            jFCExpendItem3.NbChildrenView = this.t2(list, jFCExpendItem3, num);
                            num += jFCExpendItem3.NbChildrenView;
                            if (jFCExpendItem3.NbChildrenView == 0 && this.LstGroupingWithRowWhenNoChildrenLevel.Contains(jFCExpendItem3.Level))
                            {
                                list.Add(jFCExpendItem3);
                                num++;
                            }
                        }
                    }
                }
                this.dataSource = list.ToArray();
                this.RefreshScrollBarVertical();
                this.OnDataSourceChanged();
            }
        }

        private int ExpandeItem(List<JFCExpendItem> lstds, JFCExpendItem item, int idxItem, PropertyInfo d)
        {
            IEnumerable tab = null;
            if (d.PropertyType.IsArray)
            {
                tab = (IEnumerable)d.GetValue(item.Obj, null);
            }
            else if (d.GetValue(item.Obj, null) is IEnumerable)
            {
                tab = (IEnumerable)d.GetValue(item.Obj, null);
            }
            return this.ExpandeItem(lstds, item, idxItem, tab);
        }

        private int ExpandeItem(List<JFCExpendItem> lstAdd, JFCExpendItem item, int idxItem, IEnumerable Tab)
        {
            int num = 0;
            if (idxItem >= -1)
            {
                int level = item.Level;
                if (item.IsExpended)
                {
                    int num2 = 0;
                    int num3 = idxItem;
                    int num4 = 0;
                    bool flag = false;
                    item.Children.Clear();
                    foreach (object item2 in Tab)
                    {
                        JFCExpendItem jFCExpendItem = new JFCExpendItem(item2);
                        jFCExpendItem.Level = (short)(level + 1);
                        jFCExpendItem.Parent = item;
                        item.Children.Add(jFCExpendItem);
                        flag = false;
                        if (this.LstGroupingWithRowLevel.Contains(jFCExpendItem.Level))
                        {
                            lstAdd.Add(jFCExpendItem);
                            jFCExpendItem.StartChildViewIndex = num3;
                            num3++;
                            num2++;
                            flag = true;
                        }
                        else if (!this.LstGroupingNoRowLevel.Contains(jFCExpendItem.Level))
                        {
                            lstAdd.Add(jFCExpendItem);
                            jFCExpendItem.StartChildViewIndex = num3;
                            num3++;
                            num2++;
                        }
                        else
                        {
                            jFCExpendItem.StartChildViewIndex = num3;
                        }
                        int num5 = this.t(lstAdd, jFCExpendItem, num3);
                        if (flag)
                        {
                            jFCExpendItem.NbChildrenView = num5 + 1;
                        }
                        else if (num5 == 0 && this.LstGroupingWithRowWhenNoChildrenLevel.Contains(jFCExpendItem.Level))
                        {
                            lstAdd.Add(jFCExpendItem);
                            jFCExpendItem.StartChildViewIndex = num3;
                            num3++;
                            num2++;
                            jFCExpendItem.NbChildrenView = num5 + 1;
                        }
                        else
                        {
                            jFCExpendItem.NbChildrenView = num5;
                        }
                        num2 += num5;
                        num3 += num5;
                        num4++;
                    }
                    this.FlagSearchObject(lstAdd);
                    return num2;
                }
                int num6 = 0;
                int num7 = idxItem + 1;
                int num8 = lstAdd.Count();
                while (lstAdd[num7].Level > item.Level)
                {
                    num6++;
                    num7++;
                    if (num7 >= num8)
                    {
                        break;
                    }
                }
                for (int i = 0; i < num6; i++)
                {
                    lstAdd[idxItem + 1 + i].ResetEvent();
                }
                lstAdd.RemoveRange(idxItem + 1, num6);
                return -num6;
            }
            return 0;
        }

        private int ExpandeItem2(List<JFCExpendItem> lstds, JFCExpendItem item, int idxItem, IEnumerable Tab)
        {
            int num = 0;
            if (idxItem >= -1)
            {
                num = idxItem;
            }
            else
            {
                num = lstds.IndexOf(item);
                if (num == -1)
                {
                    return 0;
                }
            }
            short level = item.Level;
            if (item.IsExpended)
            {
                int num2 = 0;
                int num3 = num;
                int num4 = 0;
                bool flag = false;
                item.Children.Clear();
                {
                    foreach (object item2 in Tab)
                    {
                        object obj = item2;
                        JFCExpendItem jFCExpendItem = null;
                        flag = false;
                        jFCExpendItem = lstds[num3];
                        item.Children.Add(jFCExpendItem);
                        if (this.LstGroupingWithRowLevel.Contains(level + 1))
                        {
                            if (jFCExpendItem.Level != level + 1)
                            {
                                jFCExpendItem.Level = (short)(level + 1);
                            }
                            if (jFCExpendItem.Parent != item)
                            {
                                jFCExpendItem.Parent = item;
                            }
                            num3++;
                            num2++;
                            flag = true;
                        }
                        else if (this.LstGroupingWithRowWhenNoChildrenLevel.Contains(level + 1) && lstds[num3].NbChildrenView == 0)
                        {
                            if (jFCExpendItem.Level != level + 1)
                            {
                                jFCExpendItem.Level = (short)(level + 1);
                            }
                            if (jFCExpendItem.Parent != item)
                            {
                                jFCExpendItem.Parent = item;
                            }
                            num3++;
                            num2++;
                            flag = true;
                        }
                        else if (!this.LstGroupingNoRowLevel.Contains(level + 1))
                        {
                            if (jFCExpendItem.Level != level + 1)
                            {
                                jFCExpendItem.Level = (short)(level + 1);
                            }
                            if (jFCExpendItem.Parent != item)
                            {
                                jFCExpendItem.Parent = item;
                            }
                            num3++;
                            num2++;
                        }
                        else
                        {
                            while (jFCExpendItem.Level != level + 1 && jFCExpendItem.Parent != null)
                            {
                                jFCExpendItem = jFCExpendItem.Parent;
                            }
                            jFCExpendItem.Parent = item;
                        }
                        int num5 = this.t2(lstds, jFCExpendItem, num3);
                        if (flag)
                        {
                            jFCExpendItem.StartChildViewIndex = num3 - 1;
                            jFCExpendItem.NbChildrenView = num5 + 1;
                        }
                        else
                        {
                            jFCExpendItem.StartChildViewIndex = num3;
                            jFCExpendItem.NbChildrenView = num5;
                        }
                        num2 += num5;
                        num3 += num5;
                        num4++;
                    }
                    return num2;
                }
            }
            return 0;
        }

        private static void UpdateDatasourceFooter(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid jFCGrid = obj as JFCGrid;
            if (jFCGrid.DataSourceFooter != null)
            {
                jFCGrid.dataSourceFooter = new JFCExpendItem[jFCGrid.DataSourceFooter.Cast<object>().Count()];
                int num = 0;
                foreach (object item in jFCGrid.DataSourceFooter)
                {
                    jFCGrid.dataSourceFooter[num] = new JFCExpendItem(item);
                    num++;
                }
            }
            else
            {
                jFCGrid.dataSourceFooter = null;
            }
            JFCGrid.UpdateEcart(obj, e);
        }

        private static void UpdateRowHeight(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid jFCGrid = obj as JFCGrid;
            int num = jFCGrid.Grid11.RowDefinitions.Count();
            for (int i = 0; i < num; i++)
            {
                jFCGrid.Grid10.RowDefinitions[i].Height = new GridLength((double)jFCGrid.RowHeight);
                jFCGrid.Grid11.RowDefinitions[i].Height = new GridLength((double)jFCGrid.RowHeight);
                jFCGrid.Grid12.RowDefinitions[i].Height = new GridLength((double)jFCGrid.RowHeight);
                jFCGrid.Grid20.RowDefinitions[i].Height = new GridLength((double)jFCGrid.RowHeight);
                jFCGrid.Grid21.RowDefinitions[i].Height = new GridLength((double)jFCGrid.RowHeight);
            }
        }

        private static void UpdateHorizontalBorderColor(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            (obj as JFCGrid).Manager.RefreshHorizontalBorderColor();
        }

        private static void UpdateVerticalBorderColor(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            (obj as JFCGrid).Manager.RefreshVerticalBorderColor();
        }

        private static void UpdateContextMenuRow(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            (obj as JFCGrid).Manager.RefreshContextMenuRow();
        }

        private static void UpdateHiliteString(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid.FindString(obj, (string)e.NewValue);
        }

        private static void UpdateChildrenName(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid obj2 = obj as JFCGrid;
            JFCGrid.UpdateDatasource(obj2, default(DependencyPropertyChangedEventArgs));
            obj2.Manager.RowRefreshDataAll();
        }

        private static void UpdateExpandeChildrenName(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid obj2 = obj as JFCGrid;
            JFCGrid.UpdateDatasource(obj2, default(DependencyPropertyChangedEventArgs));
            obj2.Manager.RowRefreshDataAll();
        }

        private static void UpdateEcart(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid jFCGrid = obj as JFCGrid;
            if (jFCGrid.Manager.NbColumn(JFCGridColumn.FrozenType.Start) == 0)
            {
                jFCGrid.ColumnHeader.Width = new GridLength(0.0);
                jFCGrid.ColumnEcart1.Width = new GridLength(0.0);
            }
            else
            {
                jFCGrid.ColumnHeader.Width = default(GridLength);
                jFCGrid.ColumnEcart1.Width = jFCGrid.EcartHeaderRow;
            }
            if (jFCGrid.Manager.NbColumn(JFCGridColumn.FrozenType.End) == 0)
            {
                jFCGrid.ColFooter.Width = new GridLength(0.0);
                jFCGrid.ColumnEcart2.Width = new GridLength(0.0);
            }
            else
            {
                jFCGrid.ColumnEcart2.Width = jFCGrid.EcartBodyFooterVertical;
                jFCGrid.ColFooter.Width = default(GridLength);
            }
            jFCGrid.ColumnEcart3.Width = jFCGrid.EcartScrollVertical;
            jFCGrid.RowEcart1.Height = jFCGrid.EcartHeaderColumn;
            if (jFCGrid.dataSourceFooter == null || jFCGrid.dataSourceFooter.Count() == 0)
            {
                jFCGrid.RowFooter.Height = new GridLength(0.0);
                jFCGrid.RowEcart2.Height = new GridLength(0.0);
            }
            else
            {
                jFCGrid.RowFooter.Height = default(GridLength);
                jFCGrid.RowEcart2.Height = jFCGrid.EcartBodyFooterHorizontal;
            }
            jFCGrid.RowEcart3.Height = jFCGrid.EcartScrollHorizontal;
        }

        private static void FindString(DependencyObject element, string stringFind)
        {
            int childrenCount = VisualTreeHelper.GetChildrenCount(element);
            for (int i = 0; i < childrenCount; i++)
            {
                VisualTreeHelper.GetChild(element, i);
            }
        }

        private static void UpdateSearchButtonVisibility(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid jFCGrid = obj as JFCGrid;
            if ((bool)e.NewValue)
            {
                jFCGrid.SearchBorder.Visibility = Visibility.Visible;
            }
            else
            {
                jFCGrid.SearchBorder.Visibility = Visibility.Collapsed;
            }
        }

        internal int NbRowAffichable()
        {
            if (base.VerticalAlignment == VerticalAlignment.Stretch)
            {
                if (!double.IsInfinity(this.MaxSize.Height))
                {
                    if (this.MaxSize.Height != 0.0)
                    {
                        int num = 0;
                        int num2 = (int)this.ScrollViewerBody.GetValue(Grid.RowProperty);
                        double num3 = 0.0;
                        double num4 = 0.0;
                        for (int i = 0; i < num2; i++)
                        {
                            num3 += this.TotalGrid.RowDefinitions[i].ActualHeight;
                        }
                        for (int j = num2 + 1; j < this.TotalGrid.RowDefinitions.Count; j++)
                        {
                            num4 += this.TotalGrid.RowDefinitions[j].ActualHeight;
                        }
                        double num5 = this.MaxSize.Height - num3 - num4;
                        num = (int)(num5 / (double)this.RowHeight);
                        if (num5 % (double)this.RowHeight != 0.0)
                        {
                            num++;
                        }
                        if (num < 0)
                        {
                            num = 0;
                        }
                        return num;
                    }
                    int index = (int)this.ScrollViewerBody.GetValue(Grid.RowProperty);
                    this.TotalGrid.RowDefinitions[index].Height = new GridLength(1.0, GridUnitType.Star);
                    return 0;
                }
                int index2 = (int)this.ScrollViewerBody.GetValue(Grid.RowProperty);
                if (this.dataSource != null)
                {
                    if (this.dataSource.Count() < this.NbRowMax)
                    {
                        this.TotalGrid.RowDefinitions[index2].Height = new GridLength((double)(this.dataSource.Count() * this.RowHeight));
                    }
                    else
                    {
                        this.TotalGrid.RowDefinitions[index2].Height = new GridLength((double)(this.NbRowMax * this.RowHeight));
                    }
                }
                else
                {
                    this.TotalGrid.RowDefinitions[index2].Height = new GridLength(0.0);
                }
                return this.NbRowMax;
            }
            int index3 = (int)this.ScrollViewerBody.GetValue(Grid.RowProperty);
            if (this.dataSource != null)
            {
                if (this.dataSource.Count() < this.NbRowMax)
                {
                    this.TotalGrid.RowDefinitions[index3].Height = new GridLength((double)(this.dataSource.Count() * this.RowHeight));
                }
                else
                {
                    this.TotalGrid.RowDefinitions[index3].Height = new GridLength((double)(this.NbRowMax * this.RowHeight));
                }
            }
            else
            {
                this.TotalGrid.RowDefinitions[index3].Height = new GridLength(0.0);
            }
            return this.NbRowMax;
        }

        private int NbRowAffichableEntiere()
        {
            if (base.VerticalAlignment == VerticalAlignment.Stretch)
            {
                if (!double.IsInfinity(this.MaxSize.Height))
                {
                    if (this.MaxSize.Height != 0.0)
                    {
                        int num = (int)this.ScrollViewerBody.GetValue(Grid.RowProperty);
                        double num2 = 0.0;
                        double num3 = 0.0;
                        for (int i = 0; i < num; i++)
                        {
                            num2 += this.TotalGrid.RowDefinitions[i].ActualHeight;
                        }
                        for (int j = num + 1; j < this.TotalGrid.RowDefinitions.Count; j++)
                        {
                            num3 += this.TotalGrid.RowDefinitions[j].ActualHeight;
                        }
                        return (int)((this.MaxSize.Height - num2 - num3) / (double)this.RowHeight);
                    }
                    int index = (int)this.ScrollViewerBody.GetValue(Grid.RowProperty);
                    this.TotalGrid.RowDefinitions[index].Height = new GridLength(1.0, GridUnitType.Star);
                    return 0;
                }
                int index2 = (int)this.ScrollViewerBody.GetValue(Grid.RowProperty);
                if (this.dataSource != null)
                {
                    if (this.dataSource.Count() < this.NbRowMax)
                    {
                        this.TotalGrid.RowDefinitions[index2].Height = new GridLength((double)(this.dataSource.Count() * this.RowHeight));
                    }
                    else
                    {
                        this.TotalGrid.RowDefinitions[index2].Height = new GridLength((double)(this.NbRowMax * this.RowHeight));
                    }
                }
                else
                {
                    this.TotalGrid.RowDefinitions[index2].Height = new GridLength(0.0);
                }
                return this.NbRowMax;
            }
            int index3 = (int)this.ScrollViewerBody.GetValue(Grid.RowProperty);
            if (this.dataSource != null)
            {
                if (this.dataSource.Count() < this.NbRowMax)
                {
                    this.TotalGrid.RowDefinitions[index3].Height = new GridLength((double)(this.dataSource.Count() * this.RowHeight));
                }
                else
                {
                    this.TotalGrid.RowDefinitions[index3].Height = new GridLength((double)(this.NbRowMax * this.RowHeight));
                }
            }
            else
            {
                this.TotalGrid.RowDefinitions[index3].Height = new GridLength(0.0);
            }
            return this.NbRowMax;
        }

        internal int NbColumnAffichable()
        {
            if (base.HorizontalAlignment == HorizontalAlignment.Stretch)
            {
                if (!double.IsInfinity(this.MaxSize.Width))
                {
                    if (this.MaxSize.Width != 0.0)
                    {
                        int num = 0;
                        double num2 = 0.0;
                        int num3 = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                        double num4 = 0.0;
                        double num5 = 0.0;
                        for (int i = 0; i < num3; i++)
                        {
                            num4 += this.TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }
                        for (int j = num3 + 1; j < this.TotalGrid.ColumnDefinitions.Count; j++)
                        {
                            num5 += this.TotalGrid.ColumnDefinitions[j].ActualWidth;
                        }
                        num2 = this.MaxSize.Width - num4 - num5;
                        double num6 = 0.0;
                        for (int k = (int)this.ScrollBarHorizontal.Value; k < this.Manager.NbColumn(JFCGridColumn.FrozenType.None); k++)
                        {
                            if (!(num6 < num2))
                            {
                                break;
                            }
                            num6 += this.Manager.ColumnWidth(JFCGridColumn.FrozenType.None, k);
                            num++;
                        }
                        if (num < 0)
                        {
                            num = 0;
                        }
                        return num;
                    }
                    int index = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                    this.TotalGrid.ColumnDefinitions[index].Width = new GridLength(1.0, GridUnitType.Star);
                    return 0;
                }
                int index2 = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                if (this.Manager.NbColumn(JFCGridColumn.FrozenType.None) > 0)
                {
                    int result = this.Manager.NbColumn(JFCGridColumn.FrozenType.None) - (int)this.ScrollBarHorizontal.Value;
                    this.TotalGrid.ColumnDefinitions[index2].Width = default(GridLength);
                    return result;
                }
                this.TotalGrid.ColumnDefinitions[index2].Width = new GridLength(0.0);
                return 0;
            }
            int num7 = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
            return 0;
        }

        private int NbColumnAffichableEntiere()
        {
            if (base.HorizontalAlignment == HorizontalAlignment.Stretch)
            {
                if (!double.IsInfinity(this.MaxSize.Width))
                {
                    if (this.MaxSize.Width != 0.0)
                    {
                        int num = 0;
                        double num2 = 0.0;
                        int num3 = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                        double num4 = 0.0;
                        double num5 = 0.0;
                        for (int i = 0; i < num3; i++)
                        {
                            num4 += this.TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }
                        for (int j = num3 + 1; j < this.TotalGrid.ColumnDefinitions.Count; j++)
                        {
                            num5 += this.TotalGrid.ColumnDefinitions[j].ActualWidth;
                        }
                        num2 = this.MaxSize.Width - num4 - num5;
                        double num6 = 0.0;
                        for (int k = (int)this.ScrollBarHorizontal.Value; k < this.Manager.NbColumn(JFCGridColumn.FrozenType.None); k++)
                        {
                            if (!(num6 < num2))
                            {
                                break;
                            }
                            num6 += this.Manager.ColumnWidth(JFCGridColumn.FrozenType.None, k);
                            num++;
                        }
                        if (num6 > num2)
                        {
                            num--;
                        }
                        if (num < 0)
                        {
                            num = 0;
                        }
                        return num;
                    }
                    int index = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                    this.TotalGrid.ColumnDefinitions[index].Width = new GridLength(1.0, GridUnitType.Star);
                    return 0;
                }
                int index2 = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                if (this.Manager.NbColumn(JFCGridColumn.FrozenType.None) > 0)
                {
                    int result = this.Manager.NbColumn(JFCGridColumn.FrozenType.None) - (int)this.ScrollBarHorizontal.Value;
                    this.TotalGrid.ColumnDefinitions[index2].Width = default(GridLength);
                    return result;
                }
                this.TotalGrid.ColumnDefinitions[index2].Width = new GridLength(0.0);
                return 0;
            }
            return 0;
        }

        private int NbColumnAffichableEntiereFin()
        {
            if (base.HorizontalAlignment == HorizontalAlignment.Stretch)
            {
                if (!double.IsInfinity(this.MaxSize.Width))
                {
                    if (this.MaxSize.Width != 0.0)
                    {
                        int num = 0;
                        double num2 = 0.0;
                        int num3 = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                        double num4 = 0.0;
                        double num5 = 0.0;
                        for (int i = 0; i < num3; i++)
                        {
                            num4 += this.TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }
                        for (int j = num3 + 1; j < this.TotalGrid.ColumnDefinitions.Count; j++)
                        {
                            num5 += this.TotalGrid.ColumnDefinitions[j].ActualWidth;
                        }
                        num2 = this.MaxSize.Width - num4 - num5;
                        double num6 = 0.0;
                        int num7 = this.Manager.NbColumn(JFCGridColumn.FrozenType.None) - 1;
                        while (num7 >= 0 && num6 < num2)
                        {
                            num6 += this.Manager.ColumnWidth(JFCGridColumn.FrozenType.None, num7);
                            num++;
                            num7--;
                        }
                        if (num6 > num2)
                        {
                            num--;
                        }
                        if (num < 0)
                        {
                            num = 0;
                        }
                        return num;
                    }
                    int index = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                    this.TotalGrid.ColumnDefinitions[index].Width = new GridLength(1.0, GridUnitType.Star);
                    return 0;
                }
                int index2 = (int)this.ScrollViewerBody.GetValue(Grid.ColumnProperty);
                if (this.Manager.NbColumn(JFCGridColumn.FrozenType.None) > 0)
                {
                    int result = this.Manager.NbColumn(JFCGridColumn.FrozenType.None) - (int)this.ScrollBarHorizontal.Value;
                    this.TotalGrid.ColumnDefinitions[index2].Width = default(GridLength);
                    return result;
                }
                this.TotalGrid.ColumnDefinitions[index2].Width = new GridLength(0.0);
                return 0;
            }
            return 0;
        }

        private void OnDataSourceChanged()
        {
            if (this.dataSource != null)
            {
                int i = this.Grid11.RowDefinitions.Count();
                int num = this.dataSource.Count();
                int num2 = this.NbRowAffichable();
                JFCExpendItem[] array = this.SelectedExpendItems.ToArray();
                foreach (JFCExpendItem jFCExpendItem in array)
                {
                    if (!this.dataSource.Contains(jFCExpendItem))
                    {
                        this.SelectedExpendItems.Remove(jFCExpendItem);
                    }
                }
                if (i < num2 && i < num)
                {
                    for (; i < num2 && i < num; i++)
                    {
                        this.Manager.RowAdd(false);
                    }
                }
                this.Manager.RowRefreshDataAll();
            }
            else
            {
                this.Manager.RowRemoveAll();
            }
        }

        private void OnDataSourceFooterChanged()
        {
            if (this.dataSourceFooter != null)
            {
                this.Manager.RowFooterRemoveAll();
                if (this.dataSourceFooter != null)
                {
                    for (int i = 0; i < this.dataSourceFooter.Count(); i++)
                    {
                        this.Manager.RowFooterAdd(true);
                    }
                }
            }
            else
            {
                this.Manager.RowFooterRemoveAll();
            }
        }

        private void ScrollBarHorizontal_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!this.IsVirtualised)
            {
                int num = (int)this.ScrollBarHorizontal.Value;
                this.ScrollViewerBody.ScrollToHorizontalOffset((double)num);
                this.ScrollViewerBodyFooter.ScrollToHorizontalOffset((double)num);
                this.ScrollViewerHeaderColumn.ScrollToHorizontalOffset((double)num);
            }
            else
            {
                this.ScrollHorizontalValue = (int)e.NewValue;
            }
        }

        private void ScrollHorizontalValueChanged(int OldValue, int NewValue)
        {
            if (this.ColunmDefinition.Count() > 0)
            {
                if (this.isVirtualised)
                {
                    if (this.Manager.NbColumn(JFCGridColumn.FrozenType.None) > 0)
                    {
                        int nbCol = this.NbColumnAffichable();
                        this.Manager.SetStartIndexAndNbCol(NewValue, nbCol);
                    }
                    else
                    {
                        this.Manager.ColumnRemoveAll(false);
                    }
                }
                else
                {
                    this.Manager.NbColView = 0;
                    this.Manager.SetStartIndexAndNbCol(0, 2147483647);
                }
                this.RefreshScrollBarHorizontal(false);
            }
            else
            {
                this.Manager.ColumnRemoveAll();
            }
        }

        private void ScrollBarVertical_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            this.ScrollVerticalValue = (int)e.NewValue;
        }

        private void ScrollVerticalValueChanged(int OldValue, int NewValue)
        {
            int value = NewValue - OldValue;
            int num = this.Grid11.RowDefinitions.Count();
            int num2 = this.NbRowAffichable();
            if (this.dataSource != null)
            {
                if (Math.Abs(value) > 4)
                {
                    if (this.scrollVerticalValue + num > this.dataSource.Count())
                    {
                        for (int num3 = num; num3 > this.dataSource.Count() - this.scrollVerticalValue; num3--)
                        {
                            this.Manager.RowRemove(num3 - 1);
                        }
                    }
                    else if (num < num2)
                    {
                        for (int i = num; i < num2; i++)
                        {
                            this.Manager.RowAdd(false);
                        }
                    }
                    this.Manager.RowRefreshDataAll();
                }
                else if (this.scrollVerticalValue + num > this.dataSource.Count())
                {
                    this.Manager.RowRemove(0);
                    this.Manager.RowRefreshDataAll();
                }
                else if (OldValue < NewValue)
                {
                    this.Manager.RowMoveBeginToEnd(NewValue - OldValue);
                    if (num < num2)
                    {
                        this.Manager.RowAdd(true);
                    }
                }
                else if (OldValue > NewValue)
                {
                    this.Manager.RowMoveEndToBegin(OldValue - NewValue);
                    if (num < num2)
                    {
                        this.Manager.RowAdd(true);
                    }
                }
            }
            else
            {
                this.Manager.RowRemoveAll();
            }
        }

        private void RefreshScrollBarHorizontal()
        {
            if (!this.IsVirtualised)
            {
                Binding binding = new Binding("ScrollableWidth");
                binding.Source = this.ScrollViewerBody;
                binding.Mode = BindingMode.OneWay;
                this.ScrollBarHorizontal.SetBinding(RangeBase.MaximumProperty, binding);
                Binding binding2 = new Binding("ViewportWidth");
                binding2.Source = this.ScrollViewerBody;
                binding2.Mode = BindingMode.OneWay;
                this.ScrollBarHorizontal.SetBinding(ScrollBar.ViewportSizeProperty, binding2);
                Binding binding3 = new Binding("ViewportWidth");
                binding3.Source = this.ScrollViewerBody;
                binding3.Mode = BindingMode.OneWay;
                this.ScrollBarHorizontal.SetBinding(RangeBase.LargeChangeProperty, binding3);
                Binding binding4 = new Binding("ViewportWidth");
                binding4.Source = this.ScrollViewerBody;
                binding4.Mode = BindingMode.OneWay;
                this.ScrollBarHorizontal.SetBinding(RangeBase.SmallChangeProperty, binding4);
            }
            else
            {
                this.RefreshScrollBarHorizontal(true);
            }
        }

        internal void RefreshScrollBarHorizontal(bool UpdateNbColumn)
        {
            if (this.IsVirtualised)
            {
                double num = 0.0;
                int num2 = this.Manager.NbColumn(JFCGridColumn.FrozenType.None);
                if (num2 != 0)
                {
                    num = (double)num2;
                }
                if (num <= 0.0 || double.IsNaN(num))
                {
                    this.ScrollBarHorizontal.Minimum = 0.0;
                    this.ScrollBarHorizontal.Maximum = 0.0;
                    this.ScrollBarHorizontal.Value = 0.0;
                }
                else
                {
                    int num3 = this.NbColumnAffichableEntiere();
                    int num4 = this.NbColumnAffichableEntiereFin();
                    if (num3 == 0)
                    {
                        num3 = 1;
                    }
                    if (num4 == 0)
                    {
                        num4 = 1;
                    }
                    this.ScrollBarHorizontal.Minimum = 0.0;
                    this.ScrollBarHorizontal.Maximum = num - (double)num4;
                    this.ScrollBarHorizontal.ViewportSize = 1.0;
                    this.ScrollBarHorizontal.SmallChange = 1.0;
                    if (UpdateNbColumn)
                    {
                        if (this.IsVirtualised)
                        {
                            int nbColView = this.NbColumnAffichable();
                            this.Manager.NbColView = nbColView;
                        }
                        else
                        {
                            this.Manager.NbColView = 2147483647;
                        }
                    }
                    if (num3 > 0)
                    {
                        this.ScrollBarHorizontal.LargeChange = (double)(num3 - 1);
                    }
                }
            }
            this.RefreshScrollBarHorizontalVisibility();
        }

        private void RefreshScrollBarVertical()
        {
            double num = 0.0;
            if (this.dataSource != null)
            {
                int num2 = this.NbRowAffichableEntiere();
                num = (double)(this.dataSource.Count() - num2);
            }
            if (num <= 0.0 || double.IsNaN(num))
            {
                this.ScrollBarVertical.Minimum = 0.0;
                this.ScrollBarVertical.Maximum = 0.0;
                this.ScrollBarVertical.Value = 0.0;
            }
            else
            {
                int num3 = this.NbRowAffichable();
                if (this.ScrollBarVertical.Value > num)
                {
                    this.ScrollBarVertical.Value = num;
                }
                this.ScrollBarVertical.Minimum = 0.0;
                this.ScrollBarVertical.Maximum = num;
                this.ScrollBarVertical.ViewportSize = (double)num3;
                this.ScrollBarVertical.SmallChange = 1.0;
                if (num3 > 0)
                {
                    this.ScrollBarVertical.LargeChange = (double)(num3 - 1);
                }
            }
            this.RefreshScrollBarVerticalVisibility();
        }

        private void ScrollViewerBody_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            this.Manager.RefreshSizeColumnStar();
            this.RefreshScrollBarVertical();
            this.RefreshScrollBarHorizontal();
            if (this.dataSource != null)
            {
                Size size = e.PreviousSize;
                double height = size.Height;
                size = e.NewSize;
                if (height < size.Height)
                {
                    if (this.Grid11.RowDefinitions.Count() < this.dataSource.Count())
                    {
                        int num = this.NbRowAffichable();
                        bool flag = false;
                        bool flag2 = false;
                        if (this.ScrollBarVertical.Value == this.ScrollBarVertical.Maximum)
                        {
                            flag = true;
                        }
                        for (int i = this.Grid11.RowDefinitions.Count(); i < num; i++)
                        {
                            if (!flag)
                            {
                                this.Manager.RowAdd(false);
                            }
                            else
                            {
                                this.Manager.RowAdd(false);
                            }
                            flag2 = true;
                        }
                        if (flag2)
                        {
                            this.Manager.RowRefreshDataAll();
                        }
                    }
                }
                else
                {
                    int num2 = this.NbRowAffichable();
                    for (int num3 = this.Grid11.RowDefinitions.Count() - 1; num3 > num2; num3--)
                    {
                        this.Manager.RowRemove(num3);
                    }
                }
            }
            this.RefreshScrollBarHorizontalVisibility();
        }

        private void ScrollViewer_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
        {
            int num = 4;
            if (e.Delta > 0)
            {
                this.ScrollBarVertical.Value = this.ScrollBarVertical.Value - (double)num;
            }
            else
            {
                this.ScrollBarVertical.Value = this.ScrollBarVertical.Value + (double)num;
            }
            e.Handled = true;
        }

        public void ResetHeadersSorting()
        {
            this.ResetSorting(this.ColunmDefinition);
        }

        private void ResetSorting(JFCGridControl.ObservableCollection<JFCGridColumn> Columns)
        {
            foreach (JFCGridColumn Column in Columns)
            {
                Column.Header.IsSort = JFCGridColumnHeader.SortType.None;
                this.ResetSorting(Column.ChildrenColumns);
            }
        }

        private void ScrollViewer_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            int num = 1;
            int num2 = 5;
            if (e.Key == Key.Down)
            {
                if (e.IsDown)
                {
                    if (!e.IsRepeat)
                    {
                        this.ScrollBarVertical.Value = this.ScrollBarVertical.Value + (double)num;
                    }
                    else
                    {
                        this.ScrollBarVertical.Value = this.ScrollBarVertical.Value + (double)num2;
                    }
                }
                e.Handled = true;
            }
            else if (e.Key == Key.Up)
            {
                if (e.IsDown)
                {
                    if (!e.IsRepeat)
                    {
                        this.ScrollBarVertical.Value = this.ScrollBarVertical.Value - (double)num;
                    }
                    else
                    {
                        this.ScrollBarVertical.Value = this.ScrollBarVertical.Value - (double)num2;
                    }
                }
                e.Handled = true;
            }
            else if (e.Key == Key.Next)
            {
                if (e.IsDown)
                {
                    this.ScrollBarVertical.Value = this.ScrollBarVertical.Value + (double)(this.Grid11.RowDefinitions.Count() - 1);
                }
                e.Handled = true;
            }
            else if (e.Key == Key.Prior)
            {
                if (e.IsDown)
                {
                    this.ScrollBarVertical.Value = this.ScrollBarVertical.Value - (double)(this.Grid11.RowDefinitions.Count() - 1);
                }
                e.Handled = true;
            }
            else
            {
                if ((Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl)) && Keyboard.IsKeyDown(Key.A))
                {
                    this.Manager.SelectAll();
                    e.Handled = true;
                    return;
                }
                if ((Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl)) && Keyboard.IsKeyDown(Key.Home))
                {
                    this.ScrollBarVertical.Value = 0.0;
                    e.Handled = true;
                    return;
                }
                if (!Keyboard.IsKeyDown(Key.RightCtrl) && !Keyboard.IsKeyDown(Key.LeftCtrl))
                {
                    return;
                }
                if (Keyboard.IsKeyDown(Key.End))
                {
                    this.ScrollBarVertical.Value = this.ScrollBarVertical.Maximum;
                    e.Handled = true;
                }
            }
        }

        protected override Size MeasureOverride(Size constraint)
        {
            this.MaxSize = constraint;
            return base.MeasureOverride(constraint);
        }

        private void RefreshScrollBarHorizontalVisibility()
        {
        }

        private void RefreshScrollBarVerticalVisibility()
        {
        }

        private void ScrollViewerHeaderRow_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            if (e.VerticalChange > 0.0)
            {
                this.ScrollViewerHeaderRow.ScrollToTop();
            }
        }

        private void ScrollViewerHeaderColumn_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            if (!this.IsVirtualised)
            {
                if (e.HorizontalOffset != this.ScrollBarHorizontal.Value)
                {
                    this.ScrollViewerHeaderColumn.ScrollToHorizontalOffset(this.ScrollBarHorizontal.Value);
                }
            }
            else if (e.HorizontalChange > 0.0)
            {
                this.ScrollViewerHeaderColumn.ScrollToHorizontalOffset(0.0);
            }
        }

        private void TextBoxSearch_TextChanged(object sender, TextChangedEventArgs e)
        {
            this.OnSearchChanged(this, new DependencyPropertySearchChangedEventArgs(this.TextBoxSearch.Text, this.SearchResult));
        }

        private void TextBoxSearch_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
            {
                this.SearchNext_Click(sender, new RoutedEventArgs());
            }
        }

        private void SearchClose_Click(object sender, RoutedEventArgs e)
        {
            this.IsSearchVisible = false;
        }

        private bool FlagSearchObject(IEnumerable<JFCExpendItem> lstds)
        {
            this.SearchResult.CollectionChanged -= this.SearchResult_CollectionChanged;
            for (int i = 0; i < this.SearchResult.Count(); i++)
            {
                GridPosition value = ((Collection<GridPosition>)this.SearchResult)[i];
                object elm = value.Path.Last();
                int level = value.Path.Count() - 1;
                foreach (JFCExpendItem item in lstds.Where(delegate (JFCExpendItem item)
                {
                    if (item.Obj == elm)
                    {
                        return item.Level == level;
                    }
                    return false;
                }))
                {
                    if (item.Level == 0)
                    {
                        value.Item = item;
                        ((Collection<GridPosition>)this.SearchResult)[i] = value;
                        break;
                    }
                    if (item.Parent.Obj == value.Path.ElementAt(value.Path.Count() - 2))
                    {
                        value.Item = item;
                        ((Collection<GridPosition>)this.SearchResult)[i] = value;
                        break;
                    }
                }
            }
            this.SearchResult.CollectionChanged += this.SearchResult_CollectionChanged;
            return false;
        }

        public bool ScrollVerticalTo(IEnumerable<object> lstElement)
        {
            int num = 0;
            int num2 = -1;
            bool flag = false;
            num2 = 0;
            foreach (object item in lstElement)
            {
                for (; num2 < this.dataSource.Count(); num2++)
                {
                    JFCExpendItem jFCExpendItem = this.dataSource[num2];
                    if (jFCExpendItem.Obj == item && jFCExpendItem.Level == num)
                    {
                        if (item != lstElement.Last() && !jFCExpendItem.IsExpended)
                        {
                            jFCExpendItem.IsExpended = true;
                        }
                        flag = true;
                        break;
                    }
                }
                if (!flag)
                {
                    return false;
                }
                num++;
            }
            if (flag)
            {
                this.Manager.RowRefreshDataAll();
                this.RefreshScrollBarVertical();
                return this.ScrollVerticalTo(num2);
            }
            return false;
        }

        public bool ScrollVerticalTo(int index)
        {
            int num = this.dataSource.Count();
            int num2 = this.NbRowAffichableEntiere();
            if (index <= num - num2)
            {
                this.ScrollBarVertical.Value = (double)index;
                return true;
            }
            this.ScrollBarVertical.Value = this.ScrollBarVertical.Maximum;
            return false;
        }

        public bool ScrollHorizontalTo(int index)
        {
            int num = this.Manager.NbColumn(JFCGridColumn.FrozenType.None);
            if (index < num)
            {
                this.ScrollBarHorizontal.Value = (double)index;
                return true;
            }
            this.ScrollBarHorizontal.Value = this.ScrollBarVertical.Maximum;
            return false;
        }

        public void RefreshData()
        {
            base.Dispatcher.BeginInvoke(DispatcherPriority.Normal, (Action)delegate
            {
                this.Manager.RowRefreshDataAll();
                this.Manager.RowFooterRefreshDataAll();
            });
        }

        private void SearchPrevious_Click(object sender, RoutedEventArgs e)
        {
            if (this.SearchResult.Count() > 0)
            {
                if (this.IndexSearch >= 1)
                {
                    this.IndexSearch--;
                }
                else
                {
                    this.IndexSearch = this.SearchResult.Count() - 1;
                }
                this.ScrollVerticalTo(((Collection<GridPosition>)this.SearchResult)[this.IndexSearch].Path);
                if (this.IndexSearch == -1)
                {
                    this.TextBlockNbFind.Text = "0 / 0";
                }
                else
                {
                    TextBlock textBlockNbFind = this.TextBlockNbFind;
                    int num = this.IndexSearch + 1;
                    string str = num.ToString();
                    num = this.SearchResult.Count();
                    textBlockNbFind.Text = str + " / " + num.ToString();
                }
            }
        }

        private void SearchNext_Click(object sender, RoutedEventArgs e)
        {
            if (this.SearchResult.Count() > 0)
            {
                if (this.IndexSearch < this.SearchResult.Count() - 1)
                {
                    this.IndexSearch++;
                }
                else
                {
                    this.IndexSearch = 0;
                }
                this.ScrollVerticalTo(((Collection<GridPosition>)this.SearchResult)[this.IndexSearch].Path);
                if (this.IndexSearch == -1)
                {
                    this.TextBlockNbFind.Text = "0 / 0";
                }
                else
                {
                    TextBlock textBlockNbFind = this.TextBlockNbFind;
                    int num = this.IndexSearch + 1;
                    string str = num.ToString();
                    num = this.SearchResult.Count();
                    textBlockNbFind.Text = str + " / " + num.ToString();
                }
            }
        }

        private void SearchButton_Click(object sender, RoutedEventArgs e)
        {
            if (this.IsSearchVisible)
            {
                this.IsSearchVisible = false;
            }
            else
            {
                this.IsSearchVisible = true;
            }
        }
    }
}
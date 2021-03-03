using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;


namespace JFCGridControl
{
    /// <summary>
    /// Interaction logic for JFCGrid.xaml
    /// </summary>
    public partial class JFCGrid : UserControl, INotifyPropertyChanged
    {
        Size MaxSize = new Size();

        internal JFCGridManager Manager;

        public IEnumerable DataSource
        {
            get { return (IEnumerable)GetValue(DataSourceProperty); }
            set
            {
                Manager.ResetColumnsWidth();

                SetValue(DataSourceProperty, null);
                SetValue(DataSourceProperty, value);
                OnDataSourceChanged();

                RefreshScrollBarVertical();
                RefreshScrollBarHorizontal();

                Manager.RefreshAllColumnsWidth();
                //Manager.RefreshHeaderColumnsWidth();

                Manager.RefreshSizeColumnStar();

                if (IsSearchVisible)
                    OnSearchChanged(this, new DependencyPropertySearchChangedEventArgs(TextBoxSearch.Text, SearchResult));
            }
        }

        //internal Object[] dataSource;
        internal JFCExpendItem[] dataSource;

        public IEnumerable DataSourceFooter
        {
            get { return (IEnumerable)GetValue(DataSourceFooterProperty); }
            set
            {
                Manager.ResetColumnsWidth();

                SetValue(DataSourceFooterProperty, null);
                SetValue(DataSourceFooterProperty, value);
                OnDataSourceFooterChanged();

                Manager.RefreshAllColumnsWidth();
            }
        }

        internal JFCExpendItem[] dataSourceFooter;

        public string PropertyChildrenName
        {
            get { return (string)GetValue(PropertyChildrenNameProperty); }
            set
            {
                SetValue(PropertyChildrenNameProperty, value);
            }
        }

        public string PropertyExpandeChildrenName
        {
            get { return (string)GetValue(PropertyExpandeChildrenNameProperty); }
            set
            {
                SetValue(PropertyExpandeChildrenNameProperty, value);
            }
        }

        public event ColumnMovedEventHandler ColumnMoved;
        internal void OnColumnMoved(object sender, ColumnMovedEventArgs e)
        {
            if (ColumnMoved != null)
                ColumnMoved(this, e);
        }

        // selection des lignes
        //private List<JFCExpendItem> selectedExpendItems = new List<JFCExpendItem>();
        //internal List<JFCExpendItem> SelectedExpendItems
        private HashSet<JFCExpendItem> selectedExpendItems = new HashSet<JFCExpendItem>();
        internal HashSet<JFCExpendItem> SelectedExpendItems
        {
            get { return selectedExpendItems; }
            set
            {
                selectedExpendItems = value;

                if (selectedExpendItems == null)
                    selectedExpendItems = new HashSet<JFCExpendItem>();

                Manager.RefreshSelection();

                OnSelectedItemsChanged(this, new RoutedEventArgs());
            }
        }

        private HashSet<JFCExpendItem> selectedExpendItemsChildren = new HashSet<JFCExpendItem>();
        internal HashSet<JFCExpendItem> SelectedExpendItemsChildren
        {
            get { return selectedExpendItemsChildren; }
            set
            {
                selectedExpendItemsChildren = value;

                if (selectedExpendItemsChildren == null)
                    selectedExpendItemsChildren = new HashSet<JFCExpendItem>();

                Manager.RefreshSelection();

                OnSelectedItemsChanged(this, new RoutedEventArgs());
            }
        }

        //public List<Object> SelectedItems
        //{
        //    get
        //    {
        //        var lst = from item in selectedExpendItems
        //                  select item.Obj;

        //        return lst.ToList();
        //    }
        //    set
        //    {
        //        HashSet<JFCExpendItem> selItem = new HashSet<JFCExpendItem>();

        //        if (value != null)
        //        {
        //            foreach (var obj in value)
        //            {

        //                var lst = from item in dataSource
        //                          where item.Obj == obj
        //                          select item;

        //                if (lst.Count() == 1)
        //                    selItem.Add((JFCExpendItem)lst.ElementAt(0));
        //            }
        //        }

        //        SelectedExpendItems = selItem;
        //    }
        //}



        public ObservableCollection<Object> SelectedItems
        {
            get { return (ObservableCollection<Object>)GetValue(SelectedItemsProperty); }
            set
            {
                SetValue(SelectedItemsProperty, value);
                value.CollectionChanged += Value_CollectionChanged;
            }
        }

        private void Value_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            UpdateSelectedItems(this, new DependencyPropertyChangedEventArgs(SelectedItemsProperty, null, SelectedItems));
        }

        // Using a DependencyProperty as the backing store for SelectedItems.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty SelectedItemsProperty =
            DependencyProperty.Register("SelectedItems", typeof(ObservableCollection<Object>), typeof(JFCGrid), new PropertyMetadata(new ObservableCollection<Object>(), new PropertyChangedCallback(UpdateSelectedItems)));

        public event RoutedEventHandler SelectedItemsChanged;
        internal void OnSelectedItemsChanged(object sender, RoutedEventArgs e)
        {
            var lst = from item in selectedExpendItems
                      select item.Obj;

            SelectedItems = new ObservableCollection<object>(lst.ToList());

            if (SelectedItemsChanged != null)
                SelectedItemsChanged(this, e);
        }

        // mode de selection
        public SelectionMode SelectionMode
        { get; set; }

        private int scrollVerticalValue = 0;
        public int ScrollVerticalValue
        {
            get { return scrollVerticalValue; }
            set
            {
                int OldValue = scrollVerticalValue;
                scrollVerticalValue = value;
                ScrollVerticalValueChanged(OldValue, scrollVerticalValue);
            }
        }

        private int scrollHorizontalValue = 0;
        public int ScrollHorizontalValue
        {
            get { return scrollHorizontalValue; }
            private set
            {
                int OldValue = scrollHorizontalValue;
                scrollHorizontalValue = value;
                ScrollHorizontalValueChanged(OldValue, scrollHorizontalValue);
            }
        }

        public int RowHeight
        {
            get { return (int)GetValue(RowHeightProperty); }
            set { SetValue(RowHeightProperty, value); }
        }

        public Brush HorizontalBorderColor
        {
            get { return (Brush)GetValue(HorizontalBorderColorProperty); }
            set { SetValue(HorizontalBorderColorProperty, value); }
        }

        public Brush VerticalBorderColor
        {
            get { return (Brush)GetValue(VerticalBorderColorProperty); }
            set { SetValue(VerticalBorderColorProperty, value); }
        }

        public Brush ScrollViewerBorderColor
        {
            get { return (Brush)GetValue(ScrollViewerBorderColorProperty); }
            set { SetValue(ScrollViewerBorderColorProperty, value); }
        }

        public Thickness ScrollViewerBorderThickness
        {
            get { return (Thickness)GetValue(ScrollViewerBorderThicknessProperty); }
            set { SetValue(ScrollViewerBorderThicknessProperty, value); }
        }

        public Brush ScrollViewerBackgroundColor
        {
            get { return (Brush)GetValue(ScrollViewerBackgroundColorProperty); }
            set { SetValue(ScrollViewerBackgroundColorProperty, value); }
        }

        public Brush ScrollViewerBackgroundColorHeaderColumn
        {
            get { return (Brush)GetValue(ScrollViewerBackgroundColorHeaderColumnProperty); }
            set { SetValue(ScrollViewerBackgroundColorHeaderColumnProperty, value); }
        }

        public ContextMenu ContextmenuRow
        {
            get { return (ContextMenu)GetValue(ContextmenuRowProperty); }
            set { SetValue(ContextmenuRowProperty, value); }
        }

        //
        public Boolean ColumnMovable
        {
            get { return (Boolean)GetValue(ColumnMovableProperty); }
            set { SetValue(ColumnMovableProperty, value); }
        }

        public int NbRowMax
        {
            get { return (int)GetValue(NbRowMaxProperty); }
            set { SetValue(NbRowMaxProperty, value); }
        }

        public Visibility ScrollBarVerticalVisibility
        {
            get { return (Visibility)GetValue(ScrollBarVerticalVisibilityProperty); }
            set { SetValue(ScrollBarVerticalVisibilityProperty, value); }
        }

        public Visibility ScrollBarHorizontalVisibility
        {
            get { return (Visibility)GetValue(ScrollBarHorizontalVisibilityProperty); }
            set { SetValue(ScrollBarHorizontalVisibilityProperty, value); }
        }

        public ObservableCollection<JFCGridColumn> ColunmDefinition = new ObservableCollection<JFCGridColumn>();

        public ObservableCollection<JFCGridColumn> ColunmDefinitionFooter = new ObservableCollection<JFCGridColumn>();

        //public ObservableCollection<JFCGridColumn> ColunmDefinition
        //{
        //    get { return (ObservableCollection<JFCGridColumn>)GetValue(ColunmDefinitionProperty); }
        //    set 
        //    { 
        //        SetValue(ColunmDefinitionProperty, value);
        //        ColunmDefinition.CollectionChanged += new System.Collections.Specialized.NotifyCollectionChangedEventHandler(ColunmDefinition_CollectionChanged);
        //    }
        //}

        private bool isVirtualised = true;
        public bool IsVirtualised
        {
            get { return isVirtualised; }
            set
            {
                isVirtualised = value;

                RefreshScrollBarHorizontal();

                if (isVirtualised == false)
                {
                    Manager.NbColView = 0;
                    Manager.SetStartIndexAndNbCol(0, Int32.MaxValue);
                }
            }
        }

        #region DependencyProperty

        // Using a DependencyProperty as the backing store for RowHeight.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty RowHeightProperty =
            DependencyProperty.Register("RowHeight", typeof(int), typeof(JFCGrid), new UIPropertyMetadata(18, new PropertyChangedCallback(UpdateRowHeight)));

        // Using a DependencyProperty as the backing store for DataSource.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty DataSourceProperty =
            DependencyProperty.Register("DataSource", typeof(IEnumerable), typeof(JFCGrid), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateDatasource)));

        // Using a DependencyProperty as the backing store for DataSourceFooter.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty DataSourceFooterProperty =
            DependencyProperty.Register("DataSourceFooter", typeof(IEnumerable), typeof(JFCGrid), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateDatasourceFooter)));

        // Using a DependencyProperty as the backing store for PropertyChildrenName.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty PropertyChildrenNameProperty =
            DependencyProperty.Register("PropertyChildrenName", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("", new PropertyChangedCallback(UpdateChildrenName)));

        // Using a DependencyProperty as the backing store for PropertyExpandeChildrenName.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty PropertyExpandeChildrenNameProperty =
            DependencyProperty.Register("PropertyExpandeChildrenName", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("", new PropertyChangedCallback(UpdateExpandeChildrenName)));

        // Using a DependencyProperty as the backing store for HorizontalBorderColor.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HorizontalBorderColorProperty =
            DependencyProperty.Register("HorizontalBorderColor", typeof(Brush), typeof(JFCGrid), new UIPropertyMetadata(Brushes.Transparent, new PropertyChangedCallback(UpdateHorizontalBorderColor)));

        // Using a DependencyProperty as the backing store for VerticalBorderColor.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty VerticalBorderColorProperty =
            DependencyProperty.Register("VerticalBorderColor", typeof(Brush), typeof(JFCGrid), new UIPropertyMetadata(Brushes.Transparent, new PropertyChangedCallback(UpdateVerticalBorderColor)));

        // Using a DependencyProperty as the backing store for ScrollViewerBorderColor.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ScrollViewerBorderColorProperty =
            DependencyProperty.Register("ScrollViewerBorderColor", typeof(Brush), typeof(JFCGrid), new UIPropertyMetadata(Brushes.Black));

        // Using a DependencyProperty as the backing store for ScrollViewerBorderThickness.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ScrollViewerBorderThicknessProperty =
            DependencyProperty.Register("ScrollViewerBorderThickness", typeof(Thickness), typeof(JFCGrid), new UIPropertyMetadata(new Thickness(0)));

        public static readonly DependencyProperty ScrollViewerBackgroundColorProperty =
            DependencyProperty.Register("ScrollViewerBackgroundColor", typeof(Brush), typeof(JFCGrid), new UIPropertyMetadata(Brushes.White));

        // Using a DependencyProperty as the backing store for ScrollViewerBackgroundColorHeaderColumn.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ScrollViewerBackgroundColorHeaderColumnProperty =
            DependencyProperty.Register("ScrollViewerBackgroundColorHeaderColumn", typeof(Brush), typeof(JFCGrid), new PropertyMetadata(Brushes.Transparent));

        // Using a DependencyProperty as the backing store for RowContextmenu.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ContextmenuRowProperty =
            DependencyProperty.Register("ContextmenuRow", typeof(ContextMenu), typeof(JFCGrid), new UIPropertyMetadata(null, new PropertyChangedCallback(UpdateContextMenuRow)));

        // Using a DependencyProperty as the backing store for ColumnMovable.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ColumnMovableProperty =
            DependencyProperty.Register("ColumnMovable", typeof(Boolean), typeof(JFCGrid), new UIPropertyMetadata(false));

        // Using a DependencyProperty as the backing store for NbRowMax.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty NbRowMaxProperty =
            DependencyProperty.Register("NbRowMax", typeof(int), typeof(JFCGrid), new UIPropertyMetadata(30));

        // Using a DependencyProperty as the backing store for ScrollBarVerticalVisibility.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ScrollBarVerticalVisibilityProperty =
            DependencyProperty.Register("ScrollBarVerticalVisibility", typeof(Visibility), typeof(JFCGrid), new UIPropertyMetadata(Visibility.Visible));

        // Using a DependencyProperty as the backing store for ScrollBarHorizontalVisibility.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ScrollBarHorizontalVisibilityProperty =
            DependencyProperty.Register("ScrollBarHorizontalVisibility", typeof(Visibility), typeof(JFCGrid), new UIPropertyMetadata(Visibility.Visible));


        //// Using a DependencyProperty as the backing store for ColunmDefinition.  This enables animation, styling, binding, etc...
        //public static readonly DependencyProperty ColunmDefinitionProperty =
        //    DependencyProperty.Register("ColunmDefinition", typeof(ObservableCollection<JFCGridColumn>), typeof(JFCGrid), new UIPropertyMetadata(new ObservableCollection<JFCGridColumn>(), new PropertyChangedCallback(UpdateVerticalBorderColor)));
        #endregion

        #region Traduction

        public string LabelResearch
        {
            get { return (string)GetValue(LabelResearchProperty); }
            set { SetValue(LabelResearchProperty, value); }
        }

        // Using a DependencyProperty as the backing store for LabelResearch.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty LabelResearchProperty =
            DependencyProperty.Register("LabelResearch", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("Recherche"));

        //public string LabelPreview
        //{
        //    get { return (string)GetValue(LabelPreviewProperty); }
        //    set { SetValue(LabelPreviewProperty, value); }
        //}

        //// Using a DependencyProperty as the backing store for LabelPreview.  This enables animation, styling, binding, etc...
        //public static readonly DependencyProperty LabelPreviewProperty =
        //    DependencyProperty.Register("LabelPreview", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("Précédent"));

        //public string LabelNext
        //{
        //    get { return (string)GetValue(LabelNextProperty); }
        //    set { SetValue(LabelNextProperty, value); }
        //}

        //// Using a DependencyProperty as the backing store for LabelNext.  This enables animation, styling, binding, etc...
        //public static readonly DependencyProperty LabelNextProperty =
        //    DependencyProperty.Register("LabelNext", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("Suivant"));



        public string LabelResult
        {
            get { return (string)GetValue(LabelResultProperty); }
            set { SetValue(LabelResultProperty, value); }
        }

        // Using a DependencyProperty as the backing store for LabelResult.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty LabelResultProperty =
            DependencyProperty.Register("LabelResult", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("Résultat"));



        #endregion

        public string HiliteString
        {
            get { return (string)GetValue(HiliteStringProperty); }
            set { SetValue(HiliteStringProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HiliteString.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HiliteStringProperty =
            DependencyProperty.Register("HiliteString", typeof(string), typeof(JFCGrid), new UIPropertyMetadata("", new PropertyChangedCallback(UpdateHiliteString)));


        // Ecart entre l'entete des colonnes et la grille
        public GridLength EcartHeaderColumn
        {
            get { return (GridLength)GetValue(EcartHeaderColumnProperty); }
            set { SetValue(EcartHeaderColumnProperty, value); }
        }

        // Using a DependencyProperty as the backing store for EcartHeaderColumn.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty EcartHeaderColumnProperty =
            DependencyProperty.Register("EcartHeaderColumn", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4), new PropertyChangedCallback(UpdateEcart)));


        // Ecart entre l'entete des lignes et la grille
        public GridLength EcartHeaderRow
        {
            get { return (GridLength)GetValue(EcartHeaderRowProperty); }
            set { SetValue(EcartHeaderRowProperty, value); }
        }

        // Using a DependencyProperty as the backing store for EcartHeaderRow.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty EcartHeaderRowProperty =
            DependencyProperty.Register("EcartHeaderRow", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4), new PropertyChangedCallback(UpdateEcart)));


        public GridLength EcartBodyFooterHorizontal
        {
            get { return (GridLength)GetValue(EcartBodyFooterHorizontalProperty); }
            set { SetValue(EcartBodyFooterHorizontalProperty, value); }
        }

        // Using a DependencyProperty as the backing store for EcartBodyFooterHorizontal.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty EcartBodyFooterHorizontalProperty =
            DependencyProperty.Register("EcartBodyFooterHorizontal", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4), new PropertyChangedCallback(UpdateEcart)));

        public GridLength EcartBodyFooterVertical
        {
            get { return (GridLength)GetValue(EcartBodyFooterVerticalProperty); }
            set { SetValue(EcartBodyFooterVerticalProperty, value); }
        }

        // Using a DependencyProperty as the backing store for EcartBodyFooterHorizontal.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty EcartBodyFooterVerticalProperty =
            DependencyProperty.Register("EcartBodyFooterVertical", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4), new PropertyChangedCallback(UpdateEcart)));


        // Ecart entre la grille et le scroll horizontal
        public GridLength EcartScrollHorizontal
        {
            get { return (GridLength)GetValue(EcartScrollHorizontalProperty); }
            set { SetValue(EcartScrollHorizontalProperty, value); }
        }

        // Using a DependencyProperty as the backing store for EcartScrollHorizontal.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty EcartScrollHorizontalProperty =
            DependencyProperty.Register("EcartScrollHorizontal", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4), new PropertyChangedCallback(UpdateEcart)));


        // Ecart entre la grille et le scroll vertical
        public GridLength EcartScrollVertical
        {
            get { return (GridLength)GetValue(EcartScrollVerticalProperty); }
            set { SetValue(EcartScrollVerticalProperty, value); }
        }

        // Using a DependencyProperty as the backing store for EcartScrollVertical.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty EcartScrollVerticalProperty =
            DependencyProperty.Register("EcartScrollVertical", typeof(GridLength), typeof(JFCGrid), new UIPropertyMetadata(new GridLength(4), new PropertyChangedCallback(UpdateEcart)));


        // Contextmenu à afficher sur HeaderColumn
        public ContextMenu ContextMenuHeaderColumn
        {
            get { return Grid01.ContextMenu; }
            set
            {
                Grid00.ContextMenu = value;
                Grid01.ContextMenu = value;
                Grid02.ContextMenu = value;
            }
        }

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

            //
            // Summary:
            //     Initializes a new instance of the System.Windows.DependencyPropertyChangedEventArgs
            //     class.
            //
            // Parameters:
            //   property:
            //     The identifier for the dependency property that changed.
            //
            //   oldValue:
            //     The value of the property before the change reported by the relevant event
            //     or state change.
            //
            //   newValue:
            //     The value of the property after the change reported by the relevant event
            //     or state change.
            //public DependencyPropertyLineOverChangedEventArgs()
            //{ }

            public DependencyPropertyLineOverChangedEventArgs(DependencyProperty property, object oldValue, object newValue, GridPosition Position, int IndexDataRow)
            {
                _Property = property;
                _OldValue = oldValue;
                _NewValue = newValue;
                _Position = Position;
                _IndexDataRow = IndexDataRow;
            }

            // Summary:
            //     Determines whether two specified System.Windows.DependencyPropertyChangedEventArgs
            //     objects are different.
            //
            // Parameters:
            //   left:
            //     The first System.Windows.DependencyPropertyChangedEventArgs to compare.
            //
            //   right:
            //     The second System.Windows.DependencyPropertyChangedEventArgs to compare.
            //
            // Returns:
            //     true if the two System.Windows.DependencyPropertyChangedEventArgs instances
            //     are different; otherwise, false.
            public static bool operator !=(DependencyPropertyLineOverChangedEventArgs left, DependencyPropertyLineOverChangedEventArgs right)
            {
                if (left == right)
                    return false;
                else
                    return true;
            }
            //
            // Summary:
            //     Determines whether two specified System.Windows.DependencyPropertyChangedEventArgs
            //     objects have the same value.
            //
            // Parameters:
            //   left:
            //     The first System.Windows.DependencyPropertyChangedEventArgs to compare.
            //
            //   right:
            //     The second System.Windows.DependencyPropertyChangedEventArgs to compare.
            //
            // Returns:
            //     true if the two System.Windows.DependencyPropertyChangedEventArgs instances
            //     are equivalent; otherwise, false.
            public static bool operator ==(DependencyPropertyLineOverChangedEventArgs left, DependencyPropertyLineOverChangedEventArgs right)
            {
                if (left == right)
                    return true;
                else
                    return false;
            }

            private int _IndexDataRow;
            public int IndexDataRow
            { get { return _IndexDataRow; } }

            private GridPosition _Position;
            public GridPosition Position
            { get { return _Position; } }

            // Summary:
            //     Gets the value of the property after the change.
            //
            // Returns:
            //     The property value after the change.
            private object _NewValue;
            public object NewValue
            { get { return _NewValue; } }
            //
            // Summary:
            //     Gets the value of the property before the change.
            //
            // Returns:
            //     The property value before the change.
            private object _OldValue;
            public object OldValue
            { get { return _OldValue; } }
            //
            // Summary:
            //     Gets the identifier for the dependency property where the value change occurred.
            //
            // Returns:
            //     The identifier field of the dependency property where the value change occurred.
            private DependencyProperty _Property;
            public DependencyProperty Property
            { get { return _Property; } }

            // Summary:
            //     Determines whether the provided System.Windows.DependencyPropertyChangedEventArgs
            //     is equivalent to the current System.Windows.DependencyPropertyChangedEventArgs.
            //
            // Parameters:
            //   args:
            //     The System.Windows.DependencyPropertyChangedEventArgs to compare to the current
            //     System.Windows.DependencyPropertyChangedEventArgs
            //
            // Returns:
            //     true if the provided System.Windows.DependencyPropertyChangedEventArgs is
            //     equivalent to the current System.Windows.DependencyPropertyChangedEventArgs;
            //     otherwise, false.
            public bool Equals(DependencyPropertyLineOverChangedEventArgs args)
            {
                if (this.NewValue == args.NewValue && this.OldValue == args.OldValue && this.Property == args.Property)
                    return true;
                else
                    return false;
            }

            //
            // Summary:
            //     Determines whether the provided object is equivalent to the current System.Windows.DependencyPropertyChangedEventArgs.
            //
            // Parameters:
            //   obj:
            //     The object to compare to the current System.Windows.DependencyPropertyChangedEventArgs.
            //
            // Returns:
            //     true if the provided object is equivalent to the current System.Windows.DependencyPropertyChangedEventArgs;
            //     otherwise, false.
            public override bool Equals(object obj)
            {
                if (obj is DependencyPropertyLineOverChangedEventArgs)
                {
                    DependencyPropertyLineOverChangedEventArgs dp = (DependencyPropertyLineOverChangedEventArgs)obj;
                    if (this.Equals(dp))
                        return true;
                }

                return false;
            }
            //
            // Summary:
            //     Gets a hash code for this System.Windows.DependencyPropertyChangedEventArgs.
            //
            // Returns:
            //     A signed 32-bit integer hash code.
            public override int GetHashCode()
            {
                return NewValue.GetHashCode() + OldValue.GetHashCode();
            }
        }

        // 
        public event DependencyPropertyLineOverChangedEventHandler LineOverChanged;
        internal void OnLineOverChanged(object sender, DependencyPropertyLineOverChangedEventArgs e)
        {
            if (LineOverChanged != null)
                LineOverChanged(this, e);
        }

        public struct GridPosition
        {
            public GridPosition(Object Row, IEnumerable<Object> Path, JFCGridColumn Column)
            {
                _Row = Row;
                _Path = Path;
                _Column = Column;
                _Item = null;
            }

            private Object _Row;
            public Object Row
            {
                get { return _Row; }
            }

            private IEnumerable<Object> _Path;
            public IEnumerable<Object> Path
            {
                get { return _Path; }
            }

            private JFCGridColumn _Column;
            public JFCGridColumn Column
            {
                get { return _Column; }
            }

            private JFCExpendItem _Item;
            internal JFCExpendItem Item
            {
                get { return _Item; }
                set { _Item = value; }
            }

            internal void SetItem(JFCExpendItem item)
            {
                _Item = item;
            }
        }

        internal ObservableCollection<GridPosition> SearchResult = new ObservableCollection<GridPosition>();

        private int IndexSearch = -1;

        public delegate void DependencyPropertySearchChangedEventHandler(object sender, DependencyPropertySearchChangedEventArgs e);

        public struct DependencyPropertySearchChangedEventArgs
        {
            public DependencyPropertySearchChangedEventArgs(string textSearching, ObservableCollection<GridPosition> result)
            {
                _Text = textSearching;
                _Result = result;
            }

            private string _Text;
            public string Text
            { get { return _Text; } }

            private ObservableCollection<GridPosition> _Result;
            public ObservableCollection<GridPosition> Result
            { get { return _Result; } }
        }



        public Boolean IsPossibleToSearch
        {
            get { return (Boolean)GetValue(IsPossibleToSearchProperty); }
            set { SetValue(IsPossibleToSearchProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsPossibleToSearch.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsPossibleToSearchProperty =
            DependencyProperty.Register("IsPossibleToSearch", typeof(Boolean), typeof(JFCGrid), new UIPropertyMetadata(false, new PropertyChangedCallback(UpdateSearchButtonVisibility)));



        private event DependencyPropertySearchChangedEventHandler SearchChangedInternal;
        // 
        public event DependencyPropertySearchChangedEventHandler SearchChanged
        {
            add
            {
                SearchChangedInternal += value;
                this.IsPossibleToSearch = true;
            }
            remove
            {
                SearchChangedInternal -= value;
                this.IsPossibleToSearch = false;
            }

        }


        internal void OnSearchChanged(object sender, DependencyPropertySearchChangedEventArgs e)
        {
            if (SearchChangedInternal != null)
            {
                SearchResult.Clear();


                SearchChangedInternal(this, e);
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
        internal void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, e);
        }

        private List<int> LstGroupingNoRowLevel = new List<int>();
        private List<int> LstGroupingWithRowLevel = new List<int>();
        private List<int> LstGroupingWithRowWhenNoChildrenLevel = new List<int>();

        public JFCGrid()
        {
            InitializeComponent();

            Manager = new JFCGridManager(this);
            //ColunmDefinition = new ObservableCollection<JFCGridColumn>();
            ColunmDefinition.CollectionChanged += new NotifyCollectionChangedEventHandler(ColunmDefinition_CollectionChanged);
            ColunmDefinitionFooter.CollectionChanged += new NotifyCollectionChangedEventHandler(ColunmDefinitionFooter_CollectionChanged);

            SearchResult.CollectionChanged += new NotifyCollectionChangedEventHandler(SearchResult_CollectionChanged);

            //this.KeyDown += new KeyEventHandler(JFCGrid_KeyDown);
            this.PreviewKeyDown += new KeyEventHandler(JFCGrid_PreviewKeyDown);
        }

        void JFCGrid_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Up)
                ScrollBarVertical.Value = ScrollBarVertical.Value - 1;
            else if (e.Key == Key.Down)
                ScrollBarVertical.Value = ScrollBarVertical.Value + 1;
            else if (e.Key == Key.PageUp)
                ScrollBarVertical.Value = ScrollBarVertical.Value - ScrollBarVertical.ViewportSize;
            else if (e.Key == Key.PageDown)
                ScrollBarVertical.Value = ScrollBarVertical.Value + ScrollBarVertical.ViewportSize;
        }

        //void JFCGrid_KeyDown(object sender, KeyEventArgs e)
        //{
        //    if (e.Key == Key.Up)
        //        ScrollBarVertical.Value = ScrollBarVertical.Value - 1;
        //    else if (e.Key == Key.Down)
        //        ScrollBarVertical.Value = ScrollBarVertical.Value + 1;
        //    else if (e.Key == Key.PageUp)
        //        ScrollBarVertical.Value = ScrollBarVertical.Value - ScrollBarVertical.ViewportSize;
        //    else if (e.Key == Key.PageDown)
        //        ScrollBarVertical.Value = ScrollBarVertical.Value + ScrollBarVertical.ViewportSize;
        //}

        void SearchResult_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                if (SearchResult.Count() > 0)
                {


                    Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(delegate
                                                                        {
                                                                            IndexSearch = 0;

                                                                            var position = SearchResult[IndexSearch];

                                                                            FlagSearchObject(dataSource);

                                                                            ScrollVerticalTo(position.Path);

                                                                            TextBlockNbFind.Text = (IndexSearch + 1).ToString() + " / " + SearchResult.Count().ToString();

                                                                        }));
                }
                else
                {
                    IndexSearch = -1;
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Move)
            { }
            else if (e.Action == NotifyCollectionChangedAction.Remove)
            {
                if (SearchResult.Count() > 0)
                {
                    Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(delegate
                                                                        {
                                                                            IndexSearch = 0;

                                                                            var position = SearchResult[IndexSearch];

                                                                            ScrollVerticalTo(position.Path);

                                                                            TextBlockNbFind.Text = (IndexSearch + 1).ToString() + " / " + SearchResult.Count().ToString();
                                                                        }));
                }
                else
                {
                    Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(delegate
                                                                        {
                                                                            IndexSearch = -1;
                                                                            Manager.RowRefreshDataAll();
                                                                        }));
                }

            }
            else if (e.Action == NotifyCollectionChangedAction.Replace)
            { }
            else if (e.Action == NotifyCollectionChangedAction.Reset)
            {
                IndexSearch = -1;

                Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(delegate
                                                                        {
                                                                            Manager.RowRefreshDataAll();
                                                                        }));
            }

            if (IndexSearch == -1)
                TextBlockNbFind.Text = "0 / 0";
            else
                TextBlockNbFind.Text = (IndexSearch + 1).ToString() + " / " + SearchResult.Count().ToString();

            //FlagSearchObject(dataSource);

            //SearchPrevious
            //SearchNext
        }

        private bool IsGroupingChanged = false;
        void ParamColumn(JFCGridColumn col)
        {
            if (col.TypeColumn == JFCGridColumn.TypesColumn.GroupingNoRow)
            {
                if (col.LevelGrouping != -1)
                {
                    if (!LstGroupingNoRowLevel.Contains(col.LevelGrouping))
                    {
                        LstGroupingNoRowLevel.Add(col.LevelGrouping);
                        IsGroupingChanged = true;
                    }
                }
            }
            else if (col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRow)
            {
                if (col.LevelGrouping != -1)
                {
                    if (!LstGroupingWithRowLevel.Contains(col.LevelGrouping))
                    {
                        LstGroupingWithRowLevel.Add(col.LevelGrouping);
                        IsGroupingChanged = true;
                    }
                }
            }
            else if (col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRowWhenNoChildren)
            {
                if (col.LevelGrouping != -1)
                {
                    if (!LstGroupingNoRowLevel.Contains(col.LevelGrouping))
                    {
                        LstGroupingNoRowLevel.Add(col.LevelGrouping);
                        IsGroupingChanged = true;
                    }

                    if (!LstGroupingWithRowWhenNoChildrenLevel.Contains(col.LevelGrouping))
                    {
                        LstGroupingWithRowWhenNoChildrenLevel.Add(col.LevelGrouping);
                        IsGroupingChanged = true;
                    }
                }
            }
        }

        void ColunmDefinition_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            ObservableCollection<JFCGridColumn> ListColumns = sender as ObservableCollection<JFCGridColumn>;

            Boolean RefreshHeaderColumnFrozenStart = false;
            Boolean RefreshHeaderColumnFrozenEnd = false;
            Boolean RefreshHeaderColumn = false;

            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                if (ListColumns.Parent == null)
                {
                    foreach (JFCGridColumn col in e.NewItems)
                    {

                        Manager.BuildParentColumn(col);

                        AddEvent(col);

                        if (col.ChildrenColumns.Count() != 0)
                        {

                            // on recherche les colonnes les plus basses de l'arbre
                            List<JFCGridColumn> LCol = Manager.FindChildrenColumn(col.ChildrenColumns);

                            foreach (var c in LCol)
                            {
                                Manager.ColumnAdd(c);

                                ParamColumn(c);

                                if (c.Frozen == JFCGridColumn.FrozenType.Start)
                                    RefreshHeaderColumnFrozenStart = true;
                                else if (c.Frozen == JFCGridColumn.FrozenType.End)
                                    RefreshHeaderColumnFrozenEnd = true;
                                else
                                    RefreshHeaderColumn = true;
                            }
                        }
                        else
                        {
                            Manager.ColumnAdd(col);

                            ParamColumn(col);

                            if (col.Frozen == JFCGridColumn.FrozenType.Start)
                                RefreshHeaderColumnFrozenStart = true;
                            else if (col.Frozen == JFCGridColumn.FrozenType.End)
                                RefreshHeaderColumnFrozenEnd = true;
                            else
                                RefreshHeaderColumn = true;
                        }
                    }
                }
                else if (ListColumns.Parent is JFCGridColumn)
                {
                    JFCGridColumn colParent = ListColumns.Parent as JFCGridColumn;

                    int idxColInsert = 0;

                    if (ListColumns.Count > e.NewItems.Count)
                    {
                        int nbBeforeCols = ListColumns.Count - e.NewItems.Count;

                        JFCGridColumn colBefore = ListColumns[nbBeforeCols - 1];

                        if (colBefore.ChildrenColumns.Count == 0)
                        {
                            idxColInsert = Manager.ColumnIndexOf(colBefore);

                            idxColInsert++;
                        }
                        else
                        {
                            JFCGridColumn lastCol = Manager.FindLastChildrenColumn(colBefore.ChildrenColumns);

                            idxColInsert = Manager.ColumnIndexOf(lastCol);

                            idxColInsert++;
                        }

                    }
                    else
                    {
                        idxColInsert = Manager.ColumnIndexOf(colParent);

                        // on supprime la colonne parent
                        Manager.ColumnRemove(colParent);

                        //
                        //RefreshScrollBarHorizontal();
                        Manager.NbColView = 0;
                    }

                    if (idxColInsert >= 0)
                    {
                        int idxCol = 0;

                        // on boucle sur les colonnes à ajouter
                        foreach (JFCGridColumn col in e.NewItems)
                        {

                            Manager.BuildParentColumn(col);

                            AddEvent(col);

                            if (col.ChildrenColumns.Count() != 0)
                            {

                                // on recherche les colonnes les plus basses de l'arbre
                                List<JFCGridColumn> LCol = Manager.FindChildrenColumn(col.ChildrenColumns);

                                foreach (var c in LCol)
                                {
                                    Manager.ColumnAdd(c, idxColInsert + idxCol);

                                    ParamColumn(c);

                                    if (c.Frozen == JFCGridColumn.FrozenType.Start)
                                        RefreshHeaderColumnFrozenStart = true;
                                    else if (c.Frozen == JFCGridColumn.FrozenType.End)
                                        RefreshHeaderColumnFrozenEnd = true;
                                    else
                                        RefreshHeaderColumn = true;

                                    idxCol++;
                                }
                            }
                            else
                            {
                                Manager.ColumnAdd(col, idxColInsert + idxCol);

                                ParamColumn(col);

                                if (col.Frozen == JFCGridColumn.FrozenType.Start)
                                    RefreshHeaderColumnFrozenStart = true;
                                else if (col.Frozen == JFCGridColumn.FrozenType.End)
                                    RefreshHeaderColumnFrozenEnd = true;
                                else
                                    RefreshHeaderColumn = true;

                                idxCol++;
                            }
                        }

                    }
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Move)
            {
                if (e.NewStartingIndex == e.OldStartingIndex)
                    return;

                int NbCol = ((ObservableCollection<JFCGridColumn>)sender).Count();

                foreach (JFCGridColumn col in e.OldItems)
                {

                    Manager.BuildParentColumn(col);

                    ObservableCollection<JFCGridColumn> ListColParent;

                    if (col.Parent == null)
                    {
                        ListColParent = this.ColunmDefinition;
                    }
                    else
                    {
                        ListColParent = col.Parent.ChildrenColumns;
                    }

                    var ListC = from column in ListColParent
                                where column.Frozen == col.Frozen
                                select column;


                    List<JFCGridColumn> ListCol = ListC.ToList();
                    int idxColumn = ListCol.IndexOf(col);

                    List<JFCGridColumn> Lc;
                    int idxC;

                    if (e.NewStartingIndex < e.OldStartingIndex)
                    {
                        //if (ListCol.Count() < idxColumn + 1)
                        if (idxColumn + 1 < ListCol.Count())
                        {
                            if (ListCol[idxColumn + 1].ChildrenColumns.Count() > 0)
                            {
                                Lc = Manager.FindChildrenColumn(ListCol[idxColumn + 1].ChildrenColumns);

                                idxC = Manager.ColumnIndexOf(Lc[0]);
                            }
                            else
                            {
                                idxC = Manager.ColumnIndexOf(ListCol[idxColumn + 1]);
                            }
                        }
                        else
                        {
                            idxC = Manager.NbColumn(col.Frozen) - 1;
                        }


                        if (col.ChildrenColumns.Count() > 0)
                        {
                            // on recherche les colonnes les plus basses de l'arbre
                            List<JFCGridColumn> LCol = Manager.FindChildrenColumn(col.ChildrenColumns);

                            int idx = 0;
                            foreach (var c in LCol)
                            {
                                Manager.ColumnMove(c, idxC + idx);

                                if (c.Frozen == JFCGridColumn.FrozenType.Start)
                                    RefreshHeaderColumnFrozenStart = true;
                                else if (c.Frozen == JFCGridColumn.FrozenType.End)
                                    RefreshHeaderColumnFrozenEnd = true;
                                else
                                    RefreshHeaderColumn = true;

                                idx++;
                            }
                        }
                        else
                        {
                            Manager.ColumnMove(col, idxC);

                            if (col.Frozen == JFCGridColumn.FrozenType.Start)
                                RefreshHeaderColumnFrozenStart = true;
                            else if (col.Frozen == JFCGridColumn.FrozenType.End)
                                RefreshHeaderColumnFrozenEnd = true;
                            else
                                RefreshHeaderColumn = true;
                        }
                    }
                    else if (e.NewStartingIndex > e.OldStartingIndex)
                    {
                        int idx = 0;


                        if (col.ChildrenColumns.Count() > 0)
                        {

                            if (idxColumn + 1 < ListCol.Count())
                            {
                                if (ListCol[idxColumn + 1].ChildrenColumns.Count() > 0)
                                {
                                    Lc = Manager.FindChildrenColumn(ListCol[idxColumn + 1].ChildrenColumns);

                                    idxC = Manager.ColumnIndexOf(Lc[0]);
                                }
                                else
                                {
                                    idxC = Manager.ColumnIndexOf(ListCol[idxColumn + 1]);
                                }

                                idxC--;
                            }
                            else
                            {
                                if (ListCol[idxColumn - 1].ChildrenColumns.Count() > 0)
                                {
                                    Lc = Manager.FindChildrenColumn(ListCol[idxColumn - 1].ChildrenColumns);

                                    idxC = Manager.ColumnIndexOf(Lc[Lc.Count() - 1]);
                                }
                                else
                                {
                                    idxC = Manager.ColumnIndexOf(ListCol[idxColumn - 1]);
                                }
                            }

                        }
                        else
                        {
                            if (ListCol[idxColumn - 1].ChildrenColumns.Count() == 0)
                            {
                                idxC = Manager.ColumnIndexOf(ListCol[idxColumn - 1]);
                            }
                            else
                            {
                                Lc = Manager.FindChildrenColumn(ListCol[idxColumn - 1].ChildrenColumns);
                                idxC = Manager.ColumnIndexOf(Lc[Lc.Count() - 1]);
                            }
                        }


                        if (col.ChildrenColumns.Count() > 0)
                        {
                            // on recherche les colonnes les plus basses de l'arbre
                            List<JFCGridColumn> LCol = Manager.FindChildrenColumn(col.ChildrenColumns);


                            foreach (var c in LCol)
                            {
                                Manager.ColumnMove(c, idxC);

                                if (c.Frozen == JFCGridColumn.FrozenType.Start)
                                    RefreshHeaderColumnFrozenStart = true;
                                else if (c.Frozen == JFCGridColumn.FrozenType.End)
                                    RefreshHeaderColumnFrozenEnd = true;
                                else
                                    RefreshHeaderColumn = true;

                                idx++;
                            }
                        }
                        else
                        {
                            Manager.ColumnMove(col, idxC);

                            if (col.Frozen == JFCGridColumn.FrozenType.Start)
                                RefreshHeaderColumnFrozenStart = true;
                            else if (col.Frozen == JFCGridColumn.FrozenType.End)
                                RefreshHeaderColumnFrozenEnd = true;
                            else
                                RefreshHeaderColumn = true;
                        }
                    }
                }

                if (e.NewItems[0] is JFCGridColumn)
                {
                    JFCGridColumn col = e.NewItems[0] as JFCGridColumn;
                    OnColumnMoved(this, new ColumnMovedEventArgs(col));
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove)
            {
                int indexof = 0;

                foreach (JFCGridColumn col in e.OldItems)
                {
                    RemoveEvent(col.ChildrenColumns);

                    if (col.ChildrenColumns.Count() != 0)
                    {
                        // on recherche les colonnes les plus basses de l'arbre
                        List<JFCGridColumn> LCol = Manager.FindChildrenColumn(col.ChildrenColumns);

                        foreach (var c in LCol)
                        {
                            indexof = Manager.ColumnIndexOf(c);

                            Manager.ColumnRemove(c);
                            c.ChildrenColumns.CollectionChanged -= ColunmDefinition_CollectionChanged;

                            ParamColumn(c);

                            if (c.Frozen == JFCGridColumn.FrozenType.Start)
                                RefreshHeaderColumnFrozenStart = true;
                            else if (c.Frozen == JFCGridColumn.FrozenType.End)
                                RefreshHeaderColumnFrozenEnd = true;
                            else
                                RefreshHeaderColumn = true;
                        }
                    }
                    else
                    {
                        indexof = Manager.ColumnIndexOf(col);

                        Manager.ColumnRemove(col);
                        col.ChildrenColumns.CollectionChanged -= ColunmDefinition_CollectionChanged;

                        ParamColumn(col);

                        if (col.Frozen == JFCGridColumn.FrozenType.Start)
                            RefreshHeaderColumnFrozenStart = true;
                        else if (col.Frozen == JFCGridColumn.FrozenType.End)
                            RefreshHeaderColumnFrozenEnd = true;
                        else
                            RefreshHeaderColumn = true;
                    }
                }

                if (ListColumns.Count == 0)
                {
                    JFCGridColumn col = ListColumns.Parent as JFCGridColumn;

                    if (ListColumns.Parent != null)
                    {
                        Manager.NbColView = 0;
                        Manager.ColumnAdd(col, indexof);
                    }
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Replace)
            {
                // TODO

                new Exception("Replace column is not implemented");
            }
            else if (e.Action == NotifyCollectionChangedAction.Reset)
            {
                if (ListColumns.Parent == null)
                {
                    Manager.ColumnRemoveAll();

                    LstGroupingNoRowLevel.Clear();
                    LstGroupingWithRowLevel.Clear();
                    LstGroupingWithRowWhenNoChildrenLevel.Clear();

                    RefreshHeaderColumnFrozenStart = true;
                    RefreshHeaderColumn = true;
                }
                else
                {
                    //TODO
                    //LstGroupingLevel.Clear();

                    if (((JFCGridColumn)ListColumns.Parent).Frozen == JFCGridColumn.FrozenType.Start)
                        RefreshHeaderColumnFrozenStart = true;
                    else if (((JFCGridColumn)ListColumns.Parent).Frozen == JFCGridColumn.FrozenType.End)
                        RefreshHeaderColumnFrozenEnd = true;
                    else
                        RefreshHeaderColumn = true;
                }
            }

            Manager.RefreshSizeColumnStar();

            if (RefreshHeaderColumnFrozenStart == true)
                Manager.ReBuildColumnHeader(JFCGridColumn.FrozenType.Start);

            if (RefreshHeaderColumnFrozenEnd == true)
                Manager.ReBuildColumnHeader(JFCGridColumn.FrozenType.End);

            if (RefreshHeaderColumn == true)
            {
                //Manager.ReBuildColumnHeader(false);

                RefreshScrollBarHorizontal();
            }

            UpdateEcart(this, new DependencyPropertyChangedEventArgs());

            if (IsGroupingChanged && DataSource != null)
            {
                UpdateDatasource(this, new DependencyPropertyChangedEventArgs());

                OnDataSourceChanged();

                RefreshScrollBarVertical();
                RefreshScrollBarHorizontal();

                Manager.RefreshAllColumnsWidth();

                Manager.RefreshSizeColumnStar();

                if (IsSearchVisible)
                    OnSearchChanged(this, new DependencyPropertySearchChangedEventArgs(TextBoxSearch.Text, SearchResult));
            }
        }

        void ColunmDefinitionFooter_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            ObservableCollection<JFCGridColumn> ListColumnsFooter = sender as ObservableCollection<JFCGridColumn>;

            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                //AddEventFooter((IList<JFCGridColumn>)e.NewItems);

                foreach (JFCGridColumn col in e.NewItems)
                {

                    Manager.BuildParentColumn(col);

                    AddEventFooter(col);

                    if (col.ChildrenColumns.Count() != 0)
                    {

                        // on recherche les colonnes les plus basses de l'arbre
                        List<JFCGridColumn> LCol = Manager.FindChildrenColumn(col.ChildrenColumns);

                        foreach (var c in LCol)
                        {
                            Manager.ColumnAddHeaderFooter(c);
                        }
                    }
                    else
                    {
                        Manager.ColumnAddHeaderFooter(col);
                    }
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Move)
            {
                if (e.NewStartingIndex == e.OldStartingIndex)
                    return;

                int NbCol = ((ObservableCollection<JFCGridColumn>)sender).Count();

                foreach (JFCGridColumn col in e.OldItems)
                {

                    Manager.BuildParentColumn(col);

                    ObservableCollection<JFCGridColumn> ListColParent;

                    if (col.Parent == null)
                    {
                        ListColParent = this.ColunmDefinitionFooter;
                    }
                    else
                    {
                        ListColParent = col.Parent.ChildrenColumns;
                    }

                    var ListC = from column in ListColParent
                                where column.Frozen == col.Frozen
                                select column;


                    List<JFCGridColumn> ListCol = ListC.ToList();
                    int idxColumn = ListCol.IndexOf(col);

                    List<JFCGridColumn> Lc;
                    int idxC;

                    if (e.NewStartingIndex < e.OldStartingIndex)
                    {
                        if (ListCol.Count() < idxColumn + 1)
                        {
                            if (ListCol[idxColumn + 1].ChildrenColumns.Count() > 0)
                            {
                                Lc = Manager.FindChildrenColumn(ListCol[idxColumn + 1].ChildrenColumns);

                                idxC = Manager.ColumnHeaderRowFooterIndexOf(Lc[0]);
                            }
                            else
                            {
                                idxC = Manager.ColumnHeaderRowFooterIndexOf(ListCol[idxColumn + 1]);
                            }
                        }
                        else
                        {
                            idxC = Manager.NbColumnHeaderRowFooter() - 1;
                        }


                        if (col.ChildrenColumns.Count() > 0)
                        {
                            // on recherche les colonnes les plus basses de l'arbre
                            List<JFCGridColumn> LCol = Manager.FindChildrenColumn(col.ChildrenColumns);

                            int idx = 0;
                            foreach (var c in LCol)
                            {
                                Manager.ColumnMove(c, idxC + idx);

                                idx++;
                            }
                        }
                        else
                        {
                            Manager.ColumnMove(col, idxC);
                        }
                    }
                    else if (e.NewStartingIndex > e.OldStartingIndex)
                    {
                        int idx = 0;


                        if (col.ChildrenColumns.Count() > 0)
                        {

                            if (idxColumn + 1 < ListCol.Count())
                            {
                                if (ListCol[idxColumn + 1].ChildrenColumns.Count() > 0)
                                {
                                    Lc = Manager.FindChildrenColumn(ListCol[idxColumn + 1].ChildrenColumns);

                                    idxC = Manager.ColumnIndexOf(Lc[0]);
                                }
                                else
                                {
                                    idxC = Manager.ColumnIndexOf(ListCol[idxColumn + 1]);
                                }

                                idxC--;
                            }
                            else
                            {
                                if (ListCol[idxColumn - 1].ChildrenColumns.Count() > 0)
                                {
                                    Lc = Manager.FindChildrenColumn(ListCol[idxColumn - 1].ChildrenColumns);

                                    idxC = Manager.ColumnIndexOf(Lc[Lc.Count() - 1]);
                                }
                                else
                                {
                                    idxC = Manager.ColumnIndexOf(ListCol[idxColumn - 1]);
                                }
                            }

                        }
                        else
                        {
                            if (ListCol[idxColumn - 1].ChildrenColumns.Count() == 0)
                            {
                                idxC = Manager.ColumnIndexOf(ListCol[idxColumn - 1]);
                            }
                            else
                            {
                                Lc = Manager.FindChildrenColumn(ListCol[idxColumn - 1].ChildrenColumns);
                                idxC = Manager.ColumnIndexOf(Lc[Lc.Count() - 1]);
                            }
                        }


                        if (col.ChildrenColumns.Count() > 0)
                        {
                            // on recherche les colonnes les plus basses de l'arbre
                            List<JFCGridColumn> LCol = Manager.FindChildrenColumn(col.ChildrenColumns);


                            foreach (var c in LCol)
                            {
                                Manager.ColumnMove(c, idxC);

                                idx++;
                            }
                        }
                        else
                        {
                            Manager.ColumnMove(col, idxC);
                        }
                    }
                }

                if (e.NewItems[0] is JFCGridColumn)
                {
                    JFCGridColumn col = e.NewItems[0] as JFCGridColumn;
                    OnColumnMoved(this, new ColumnMovedEventArgs(col));
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove)
            {
                Boolean RemoveOnGridHeaderStart = false;
                Boolean RemoveOnGridHeaderEnd = false;
                Boolean RemoveOnGridHeaderColumn = false;

                foreach (JFCGridColumn col in e.OldItems)
                {
                    Manager.ColumnRemove(col);
                    //col.ChildrenColumns.CollectionChanged -= ColunmDefinition_CollectionChanged;
                    RemoveEventFooter(col.ChildrenColumns);

                    if (col.Frozen == JFCGridColumn.FrozenType.Start)
                        RemoveOnGridHeaderStart = true;
                    if (col.Frozen == JFCGridColumn.FrozenType.End)
                        RemoveOnGridHeaderEnd = true;
                    else
                        RemoveOnGridHeaderColumn = true;
                }

                if (RemoveOnGridHeaderStart == true)
                    Manager.BuildHeader(Grid00);

                if (RemoveOnGridHeaderEnd == true)
                    Manager.BuildHeader(Grid02);

                if (RemoveOnGridHeaderColumn == true)
                    Manager.BuildHeader(Grid01);
            }
            else if (e.Action == NotifyCollectionChangedAction.Replace)
            {
                // TODO
            }
            else if (e.Action == NotifyCollectionChangedAction.Reset)
            {
                Manager.ColumnRemoveAll();
            }
        }

        private void AddEvent(JFCGridColumn Col)
        {
            Col.ChildrenColumns.CollectionChanged += new NotifyCollectionChangedEventHandler(ColunmDefinition_CollectionChanged);

            Col.Header.ContentChanged += new DependencyPropertyChangedEventHandler(Header_ContentChanged);

            foreach (var col in Col.ChildrenColumns)
            {
                //col.ChildrenColumns.CollectionChanged += new NotifyCollectionChangedEventHandler(ColunmDefinition_CollectionChanged);

                //col.Header.ContentChanged += new DependencyPropertyChangedEventHandler(Header_ContentChanged);

                AddEvent(col);
            }

        }

        private void RemoveEvent(ObservableCollection<JFCGridColumn> ListCol)
        {

            ListCol.CollectionChanged -= ColunmDefinition_CollectionChanged;

            foreach (var col in ListCol)
            {
                col.Header.ContentChanged -= Header_ContentChanged;

                RemoveEvent(col.ChildrenColumns);
            }

        }

        private void AddEventFooter(JFCGridColumn Col)
        {
            Col.ChildrenColumns.CollectionChanged += new NotifyCollectionChangedEventHandler(ColunmDefinition_CollectionChanged);

            Col.Header.ContentChanged += new DependencyPropertyChangedEventHandler(Header_ContentChanged);

            foreach (var col in Col.ChildrenColumns)
            {
                //col.ChildrenColumns.CollectionChanged += new NotifyCollectionChangedEventHandler(ColunmDefinition_CollectionChanged);

                //col.Header.ContentChanged += new DependencyPropertyChangedEventHandler(Header_ContentChanged);

                AddEventFooter(col);
            }

        }

        private void RemoveEventFooter(ObservableCollection<JFCGridColumn> ListCol)
        {

            ListCol.CollectionChanged -= ColunmDefinition_CollectionChanged;

            foreach (var col in ListCol)
            {
                col.Header.ContentChanged -= Header_ContentChanged;

                RemoveEventFooter(col.ChildrenColumns);
            }

        }

        //private void AddEventFooter(IList<JFCGridColumn> ListCol)
        //{

        //    //ListCol.CollectionChanged += new NotifyCollectionChangedEventHandler(ColunmDefinitionFooter_CollectionChanged);

        //    foreach (var col in ListCol)
        //    {
        //        col.ChildrenColumns.CollectionChanged += new NotifyCollectionChangedEventHandler(ColunmDefinitionFooter_CollectionChanged);

        //        col.Header.ContentChanged += new DependencyPropertyChangedEventHandler(Header_ContentChanged);

        //        AddEventFooter(col.ChildrenColumns);
        //    }

        //}

        //private void RemoveEventFooter(ObservableCollection<JFCGridColumn> ListCol)
        //{

        //    ListCol.CollectionChanged -= ColunmDefinitionFooter_CollectionChanged;

        //    foreach (var col in ListCol)
        //    {
        //        col.Header.ContentChanged -= Header_ContentChanged;

        //        RemoveEventFooter(col.ChildrenColumns);
        //    }

        //}

        internal void Header_ContentChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            JFCGridColumnHeader header = sender as JFCGridColumnHeader;

            if (header.Content != null)
                header.Column.ActualWidthHeader = Manager.CalculTailleHeaderMaxi(header.Column);
            else
                header.Column.ActualWidthHeader = new GridLength();
        }

        internal static void UpdateDatasource(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {

            JFCGrid grid = obj as JFCGrid;

            if (grid.DataSource != null)
            {
                List<JFCExpendItem> lstDS = new List<JFCExpendItem>();

                int idx = 0;
                int NbChildren = 0;

                foreach (var data in grid.DataSource)
                {
                    JFCExpendItem item = new JFCExpendItem(data);

                    //if (!(grid.LstGroupingNoRowLevel.Contains(item.Level) || grid.LstGroupingWithRowLevel.Contains(item.Level)))
                    if (!grid.LstGroupingNoRowLevel.Contains(item.Level))
                    {
                        lstDS.Add(item);

                        item.StartChildViewIndex = idx;

                        idx++;

                        if (grid.PropertyChildrenName != "")
                        {
                            NbChildren = grid.t(lstDS, item, idx);
                            item.NbChildrenView = NbChildren + 1;

                            idx += NbChildren;
                        }
                    }
                    else if (grid.PropertyChildrenName != "")
                    {
                        item.StartChildViewIndex = idx;

                        item.NbChildrenView = grid.t(lstDS, item, idx);

                        idx += item.NbChildrenView;

                        if (item.NbChildrenView == 0 && grid.LstGroupingWithRowWhenNoChildrenLevel.Contains(item.Level))
                        {
                            lstDS.Add(item);
                            idx++;
                        }
                    }
                }

                grid.dataSource = lstDS.ToArray();

            }
            else
            {
                if (grid.dataSource != null)
                {
                    foreach (var item in grid.dataSource)
                    {
                        item.ResetEvent();
                    }

                    grid.dataSource = null;
                }
            }

            grid.IsGroupingChanged = false;

            //UpdateEcart(obj, e);
        }

        private int t(List<JFCExpendItem> lstAdd, JFCExpendItem item, int idxItem)
        {
            if (item.Obj == null)
                return 0;

            //PropertyInfo d = item.Obj.GetType().GetProperty(PropertyChildrenName);

            Func<object, IEnumerable> df;
            Type t = item.Obj.GetType();
            if (!propGetter.TryGetValue(t, out df))
            {
                //df = delegate(object data)
                //{
                //    dynamic datadyn = data;
                //    return datadyn.Children;
                //};

                // Creating a parameter expression.
                var itemobj = System.Linq.Expressions.Expression.Parameter(typeof(object), "itemobj");
                var gblock = System.Linq.Expressions.Expression.Convert(
                    System.Linq.Expressions.Expression.Property(System.Linq.Expressions.Expression.Convert(itemobj, t), PropertyChildrenName),
                    typeof(IEnumerable));

                // Compile and execute an expression tree.
                df = System.Linq.Expressions.Expression.Lambda<Func<object, IEnumerable>>(gblock, itemobj).Compile();

                propGetter.Add(t, df);
            }

            if (df != null)
            {
                ////Object[] Tab = new Object[0];
                //IEnumerable Tab2 = null;

                //if (d.PropertyType.IsArray)
                //{
                //    //Tab = (Object[])d.GetValue(item.Obj, null);
                //    Tab2 = (IEnumerable)d.GetValue(item.Obj, null);

                //    //if (Tab != null)
                //    //    if (Tab.Count() > 0)
                //    //        item.IsExpendable = true;
                //}
                //else
                //{
                //    object obj = d.GetValue(item.Obj, null);

                //    if (obj is IEnumerable)
                //    {
                //        //IEnumerable ienum = (IEnumerable)d.GetValue(item.Obj, null);

                //        //if (ienum != null)
                //        //    //if (ienum.Cast<object>().Count() > 0)
                //        //    //    item.IsExpendable = true;
                //        //    Tab = ienum.Cast<object>().ToArray();

                //        Tab2 = (IEnumerable)obj;
                //    }
                //}

                IEnumerable Tab2 = df(item.Obj);

                if (Tab2 != null)
                    //if (Tab2.Cast<object>().Count() > 0)
                    if (Tab2.GetEnumerator().MoveNext())
                        item.IsExpendable = true;



                if (item.IsExpendable == true)
                {
                    if (PropertyExpandeChildrenName != null && PropertyExpandeChildrenName != "")
                    {
                        int itemAdded = 0;
                        PropertyInfo data = item.Obj.GetType().GetProperty(PropertyExpandeChildrenName);

                        if (data != null)
                        {
                            if (data.PropertyType == typeof(Boolean))
                            {

                                Boolean b = (Boolean)data.GetValue(item.Obj, null);

                                if (b == true && item.IsExpendable == true)
                                {
                                    item.IsExpended = b;
                                    itemAdded = ExpandeItem(lstAdd, item, idxItem, Tab2);
                                }
                            }

                            item.PropertyChanged -= item_PropertyChanged;
                            item.PropertyChanged += new PropertyChangedExtendedEventHandler<object>(item_PropertyChanged);

                            Binding binding = new Binding(PropertyExpandeChildrenName);
                            binding.Mode = BindingMode.TwoWay;
                            binding.Source = item.Obj;

                            //binding.Converter = new BooleanConverter();


                            BindingOperations.SetBinding(item, JFCExpendItem.IsExpendedProperty, binding);

                            return itemAdded;
                        }
                    }
                    else
                    {
                        item.PropertyChanged -= item_PropertyChanged;
                        item.PropertyChanged += new PropertyChangedExtendedEventHandler<object>(item_PropertyChanged);
                    }
                }
            }

            return 0;
        }

        //private static Dictionary<Type, PropertyInfo> propGetter = new Dictionary<Type, PropertyInfo>();
        private static Dictionary<Type, Func<object, IEnumerable>> propGetter = new Dictionary<Type, Func<object, IEnumerable>>();


        private int t2(List<JFCExpendItem> lstDS, JFCExpendItem item, int idxItem)
        {
            if (item.Obj == null)
                return 0;
            //PropertyInfo d;
            //Type t = item.Obj.GetType();
            //if (!propGetter.TryGetValue(t, out d))
            //{
            //    d = item.Obj.GetType().GetProperty(PropertyChildrenName);
            //    propGetter.Add(t, d);
            //}
            //PropertyInfo d = item.Obj.GetType().GetProperty(PropertyChildrenName);

            Func<object, IEnumerable> df;
            Type t = item.Obj.GetType();
            if (!propGetter.TryGetValue(t, out df))
            {
                //df = delegate(object data)
                //{
                //    dynamic datadyn = data;
                //    return datadyn.Children;
                //};

                // Creating a parameter expression.
                var itemobj = System.Linq.Expressions.Expression.Parameter(typeof(object), "itemobj");
                var gblock = System.Linq.Expressions.Expression.Convert(
                    System.Linq.Expressions.Expression.Property(System.Linq.Expressions.Expression.Convert(itemobj, t), PropertyChildrenName),
                    typeof(IEnumerable));

                // Compile and execute an expression tree.
                df = System.Linq.Expressions.Expression.Lambda<Func<object, IEnumerable>>(gblock, itemobj).Compile();

                propGetter.Add(t, df);
            }

            if (df != null)
            {
                ////Object[] Tab = new Object[0];
                //IEnumerable Tab2 = null;

                //if (d.PropertyType.IsArray)
                //{
                //    //Tab = (Object[])d.GetValue(item.Obj, null);
                //    Tab2 = (IEnumerable)d.GetValue(item.Obj, null);

                //    //if (Tab != null)
                //    //    if (Tab.Count() > 0)
                //    //        item.IsExpendable = true;
                //}
                //else
                //{
                //    object obj = d.GetValue(item.Obj, null);

                //    if (obj is IEnumerable)
                //    {
                //        //IEnumerable ienum = (IEnumerable)d.GetValue(item.Obj, null);

                //        //if (ienum != null)
                //        //    //if (ienum.Cast<object>().Count() > 0)
                //        //    //    item.IsExpendable = true;
                //        //    Tab = ienum.Cast<object>().ToArray();

                //        Tab2 = (IEnumerable)obj;
                //    }
                //}
                IEnumerable Tab2 = df(item.Obj);

                if (Tab2 != null)
                    //if (Tab2.Cast<object>().Count() > 0)
                    if (Tab2.GetEnumerator().MoveNext())
                        item.IsExpendable = true;

                if (item.IsExpendable == true)
                {
                    if (PropertyExpandeChildrenName != null && PropertyExpandeChildrenName != "")
                    {
                        int itemAdded = 0;
                        PropertyInfo data = item.Obj.GetType().GetProperty(PropertyExpandeChildrenName);

                        if (data != null)
                        {
                            if (data.PropertyType == typeof(Boolean))
                            {

                                Boolean b = (Boolean)data.GetValue(item.Obj, null);

                                if (b == true && item.IsExpendable == true)
                                {
                                    item.IsExpended = b;
                                    //itemAdded = ExpandeItem2(lstDS, item, idxItem, d);
                                    itemAdded = ExpandeItem2(lstDS, item, idxItem, Tab2);
                                }
                            }

                            return itemAdded;
                        }
                    }
                }
            }

            return 0;
        }

        private void UpdateNbChildren(JFCExpendItem item, int nbChildren)
        {
            if (item.Parent != null)
            {
                //item.Parent.NbChildren += item.NbChildren;
                item.Parent.NbChildrenView += nbChildren;

                UpdateNbChildren(item.Parent, nbChildren);
            }
        }

        void item_PropertyChanged(object sender, PropertyChangedExtendedEventArgs<object> e)
        {
            if (e.PropertyName == "IsExpended")
            {
                if (e.OldValue != e.NewValue)
                {
                    JFCExpendItem it = sender as JFCExpendItem;
                    PropertyInfo d = it.Obj.GetType().GetProperty(PropertyChildrenName);

                    var lstDS = dataSource.ToList();

                    int idx = lstDS.IndexOf(it);

                    if (it.IsExpended == true)
                        idx++;

                    if (it.IsExpended == true)
                    {
                        List<JFCExpendItem> lstAdd = new List<JFCExpendItem>();
                        ExpandeItem(lstAdd, it, idx, d);

                        lstDS.InsertRange(idx, lstAdd);
                    }
                    else
                    {
                        ExpandeItem(lstDS, it, idx, d);
                    }


                    if (LstGroupingNoRowLevel.Count() != 0 || LstGroupingWithRowLevel.Count() != 0)
                    {
                        // recherche le parent
                        JFCExpendItem itemParent = lstDS.First();
                        while (itemParent.Parent != null)
                        {
                            itemParent = itemParent.Parent;
                        }

                        //t2(lstDS, itemParent, itemParent.StartChildIndex);

                        int NbChildren = 0;

                        idx = 0;

                        JFCExpendItem item;

                        foreach (var data in DataSource)
                        {
                            item = null;

                            if (lstDS[idx].Obj == data)
                                item = lstDS[idx];
                            else
                            {
                                JFCExpendItem itTmp = lstDS[idx];
                                //while (itTmp.Parent != null || itTmp.Obj != data)
                                while (itTmp.Obj != data)
                                {
                                    itTmp = itTmp.Parent;

                                    if (itTmp == null)
                                        break;
                                }

                                if (itTmp == null)
                                    item = null;
                                else if (itTmp.Obj == data)
                                    item = itTmp;
                                else
                                    item = null;
                            }

                            if (!LstGroupingNoRowLevel.Contains(item.Level))
                            {
                                //lstDS.Add(item);

                                item.StartChildViewIndex = idx;

                                idx++;

                                if (PropertyChildrenName != "")
                                {
                                    NbChildren = t2(lstDS, item, idx);
                                    item.NbChildrenView = NbChildren + 1;

                                    idx += NbChildren;
                                }
                            }
                            else if (PropertyChildrenName != "")
                            {
                                item.StartChildViewIndex = idx;

                                item.NbChildrenView = t2(lstDS, item, idx);

                                idx += item.NbChildrenView;

                                if (item.NbChildrenView == 0 && LstGroupingWithRowWhenNoChildrenLevel.Contains(item.Level))
                                {
                                    lstDS.Add(item);
                                    idx++;
                                }
                            }
                        }
                    }

                    dataSource = lstDS.ToArray();

                    //Manager.RowRefreshDataAll();
                    //OnDataSourceChanged();
                    RefreshScrollBarVertical();
                    OnDataSourceChanged();
                    //Manager.RowRefreshDataAll();
                }
            }
        }

        [ValueConversion(typeof(Boolean), typeof(Boolean))]
        public class BooleanConverter : IValueConverter
        {
            public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {

                return value;
            }

            public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {
                return value;
            }
        }

        private int ExpandeItem(List<JFCExpendItem> lstds, JFCExpendItem item, int idxItem, PropertyInfo d)
        {
            //Object[] Tab = new Object[0];
            IEnumerable Tab2 = null;

            if (d.PropertyType.IsArray)
            {
                //Tab = (Object[])d.GetValue(item.Obj, null);                
                Tab2 = (IEnumerable)d.GetValue(item.Obj, null);
            }
            else
            {
                object o = d.GetValue(item.Obj, null);

                if (o is IEnumerable)
                {
                    //IEnumerable ienum = (IEnumerable)d.GetValue(item.Obj, null);

                    //Tab = ienum.Cast<object>().ToArray();
                    Tab2 = (IEnumerable)d.GetValue(item.Obj, null);
                }
            }

            return ExpandeItem(lstds, item, idxItem, Tab2);
        }

        private int ExpandeItem(List<JFCExpendItem> lstAdd, JFCExpendItem item, int idxItem, IEnumerable Tab)
        {

            int index = 0;

            if (idxItem >= -1)
                index = idxItem;
            else
            {
                //index = lstds.IndexOf(item);

                //if (index == -1)
                return 0;
            }

            int level = item.Level;

            if (item.IsExpended == true)
            {

                //Object[] Tab = new Object[0];

                //if (d.PropertyType.IsArray)
                //{
                //    Tab = (Object[])d.GetValue(item.Obj, null);
                //}
                //else
                //{
                //    object o = d.GetValue(item.Obj, null);

                //    if (o is IEnumerable)
                //    {
                //        IEnumerable ienum = (IEnumerable)d.GetValue(item.Obj, null);

                //        Tab = ienum.Cast<object>().ToArray();
                //    }
                //}

                int itemAdd = 0;
                //int idx = index + 1;
                int idx = index;

                int indexChild = 0;
                bool colGroupingWithRow = false;

                item.Children.Clear();

                foreach (var f in Tab)
                {
                    JFCExpendItem itemtmp = new JFCExpendItem(f);
                    itemtmp.Level = (short)(level + 1);

                    itemtmp.Parent = item;
                    item.Children.Add(itemtmp);

                    colGroupingWithRow = false;

                    if (LstGroupingWithRowLevel.Contains(itemtmp.Level))
                    {
                        //lstAdd.Insert(idx, itemtmp);
                        lstAdd.Add(itemtmp);

                        itemtmp.StartChildViewIndex = idx;

                        idx++;
                        itemAdd++;

                        colGroupingWithRow = true;
                    }
                    else if (!LstGroupingNoRowLevel.Contains(itemtmp.Level))
                    {
                        //lstAdd.Insert(idx, itemtmp);
                        lstAdd.Add(itemtmp);

                        itemtmp.StartChildViewIndex = idx;

                        idx++;
                        itemAdd++;
                    }
                    else
                    {
                        itemtmp.StartChildViewIndex = idx;
                    }


                    int result = t(lstAdd, itemtmp, idx);

                    if (colGroupingWithRow == true)
                    {
                        //itemtmp.StartChildIndex = idx - 1;
                        itemtmp.NbChildrenView = result + 1;
                    }
                    else
                    {
                        if (result == 0 && LstGroupingWithRowWhenNoChildrenLevel.Contains(itemtmp.Level))
                        {
                            lstAdd.Add(itemtmp);
                            itemtmp.StartChildViewIndex = idx;

                            idx++;
                            itemAdd++;

                            //itemtmp.StartChildIndex = idx - 1;
                            itemtmp.NbChildrenView = result + 1;
                        }
                        else
                        {
                            //itemtmp.StartChildIndex = idx;
                            itemtmp.NbChildrenView = result;
                        }
                    }

                    itemAdd += result;
                    idx += result;

                    indexChild++;

                    //idx++;

                }

                FlagSearchObject(lstAdd);

                return itemAdd;
            }
            else
            {
                int count = 0;
                int i = index + 1;
                int nbline = lstAdd.Count();

                while (lstAdd[i].Level > item.Level)
                {
                    count++;
                    i++;

                    if (i >= nbline)
                    {
                        break;
                    }
                }

                for (int j = 0; j < count; j++)
                {
                    var it = lstAdd[index + 1 + j];

                    it.ResetEvent();

                    it = null;
                }

                lstAdd.RemoveRange(index + 1, count);

                return -count;
            }
        }

        //private int ExpandeItem2(List<JFCExpendItem> lstds, JFCExpendItem item, int idxItem, PropertyInfo d)
        private int ExpandeItem2(List<JFCExpendItem> lstds, JFCExpendItem item, int idxItem, IEnumerable Tab)
        {

            int index = 0;

            if (idxItem >= -1)
                index = idxItem;
            else
            {
                index = lstds.IndexOf(item);

                if (index == -1)
                    return 0;
            }

            short level = item.Level;

            if (item.IsExpended == true)
            {
                int itemAdd = 0;
                //int idx = index + 1;
                int idx = index;

                int indexChild = 0;
                bool colGroupingWithRow = false;

                item.Children.Clear();

                foreach (var f in Tab)
                {
                    JFCExpendItem itemtmp = null;

                    colGroupingWithRow = false;

                    itemtmp = lstds[idx];

                    item.Children.Add(itemtmp);

                    if (LstGroupingWithRowLevel.Contains(level + 1))
                    {
                        //itemtmp = lstds[idx];

                        if (itemtmp.Level != (level + 1))
                            itemtmp.Level = (short)(level + 1);

                        if (itemtmp.Parent != item)
                            itemtmp.Parent = item;

                        idx++;
                        itemAdd++;

                        colGroupingWithRow = true;
                    }
                    else if (LstGroupingWithRowWhenNoChildrenLevel.Contains(level + 1) && lstds[idx].NbChildrenView == 0)
                    {
                        //itemtmp = lstds[idx];

                        if (itemtmp.Level != (level + 1))
                            itemtmp.Level = (short)(level + 1);

                        if (itemtmp.Parent != item)
                            itemtmp.Parent = item;

                        idx++;
                        itemAdd++;

                        colGroupingWithRow = true;
                    }
                    else if (!LstGroupingNoRowLevel.Contains(level + 1))
                    {
                        //itemtmp = lstds[idx];

                        if (itemtmp.Level != (level + 1))
                            itemtmp.Level = (short)(level + 1);

                        if (itemtmp.Parent != item)
                            itemtmp.Parent = item;

                        idx++;
                        itemAdd++;
                    }
                    else
                    {
                        //itemtmp = lstds[idx];

                        while (itemtmp.Level != level + 1)
                        {
                            if (itemtmp.Parent == null)
                                break;

                            itemtmp = itemtmp.Parent;
                        }


                        //itemtmp.Level = (short)(level + 1);

                        itemtmp.Parent = item;
                    }

                    int result = t2(lstds, itemtmp, idx);

                    if (colGroupingWithRow == true)
                    {
                        itemtmp.StartChildViewIndex = idx - 1;
                        itemtmp.NbChildrenView = result + 1;
                    }
                    else
                    {
                        itemtmp.StartChildViewIndex = idx;
                        itemtmp.NbChildrenView = result;
                    }

                    itemAdd += result;
                    idx += result;

                    indexChild++;

                    //idx++;

                }

                return itemAdd;
            }

            return 0;
        }

        private static void UpdateDatasourceFooter(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {

            JFCGrid grid = obj as JFCGrid;

            if (grid.DataSourceFooter != null)
            {
                //grid.dataSourceFooter = grid.DataSourceFooter.Cast<object>().ToArray();

                grid.dataSourceFooter = new JFCExpendItem[grid.DataSourceFooter.Cast<Object>().Count()];

                int index = 0;

                foreach (var data in grid.DataSourceFooter)
                {
                    grid.dataSourceFooter[index] = new JFCExpendItem(data);
                    index++;
                }


                //grid.RowFooter.Height = new GridLength();
            }
            else
            {
                grid.dataSourceFooter = null;
                //grid.RowFooter.Height = new GridLength(0);
            }

            UpdateEcart(obj, e);
        }

        private static void UpdateRowHeight(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            int NbRow = grid.Grid11.RowDefinitions.Count();

            for (int i = 0; i < NbRow; i++)
            {
                grid.Grid10.RowDefinitions[i].Height = new GridLength(grid.RowHeight);
                grid.Grid11.RowDefinitions[i].Height = new GridLength(grid.RowHeight);
                grid.Grid12.RowDefinitions[i].Height = new GridLength(grid.RowHeight);

                grid.Grid20.RowDefinitions[i].Height = new GridLength(grid.RowHeight);
                grid.Grid21.RowDefinitions[i].Height = new GridLength(grid.RowHeight);
            }

        }

        private static void UpdateHorizontalBorderColor(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            grid.Manager.RefreshHorizontalBorderColor();
        }

        private static void UpdateVerticalBorderColor(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            grid.Manager.RefreshVerticalBorderColor();
        }

        private static void UpdateContextMenuRow(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            grid.Manager.RefreshContextMenuRow();
        }

        private static void UpdateHiliteString(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {

            FindString(obj, (string)e.NewValue);

        }

        private static void UpdateChildrenName(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            UpdateDatasource(grid, new DependencyPropertyChangedEventArgs());
            grid.Manager.RowRefreshDataAll();
        }

        private static void UpdateExpandeChildrenName(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            UpdateDatasource(grid, new DependencyPropertyChangedEventArgs());
            grid.Manager.RowRefreshDataAll();
        }

        private static void UpdateEcart(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            #region Column

            ////////////////////////////////////////////////////////////
            /// Column

            if (grid.Manager.NbColumn(JFCGridColumn.FrozenType.Start) == 0)
            {
                grid.ColumnHeader.Width = new GridLength(0);
                grid.ColumnEcart1.Width = new GridLength(0);
            }
            else
            {
                grid.ColumnHeader.Width = new GridLength();
                grid.ColumnEcart1.Width = grid.EcartHeaderRow;
            }

            //<ColumnDefinition Width="*"/>

            if (grid.Manager.NbColumn(JFCGridColumn.FrozenType.End) == 0)
            {
                grid.ColFooter.Width = new GridLength(0);
                grid.ColumnEcart2.Width = new GridLength(0);
            }
            else
            {
                grid.ColumnEcart2.Width = grid.EcartBodyFooterVertical;
                grid.ColFooter.Width = new GridLength();
            }

            grid.ColumnEcart3.Width = grid.EcartScrollVertical;

            //grid.ColumnScrollBar" Width="Auto"/>
            #endregion

            #region row
            //<RowDefinition Height="0" x:Name="RowSearch"/>
            //<RowDefinition Height="Auto"/>

            grid.RowEcart1.Height = grid.EcartHeaderColumn;

            if (grid.dataSourceFooter == null || grid.dataSourceFooter.Count() == 0)
            {
                grid.RowFooter.Height = new GridLength(0);
                grid.RowEcart2.Height = new GridLength(0);
            }
            else
            {
                grid.RowFooter.Height = new GridLength();
                grid.RowEcart2.Height = grid.EcartBodyFooterHorizontal;
            }

            grid.RowEcart3.Height = grid.EcartScrollHorizontal;

            //<RowDefinition Height="Auto"/>
            #endregion
        }

        private static void UpdateSelectedItems(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            if (grid != null)
            {
                HashSet<JFCExpendItem> selItem = new HashSet<JFCExpendItem>();

                if (e.NewValue != null && ((IEnumerable<object>)e.NewValue).Any())
                {
                    foreach (var item in (IEnumerable<Object>)e.NewValue)
                    {

                        var lst = from it in grid.dataSource
                                  where it.Obj == item
                                  select it;

                        var tt = lst.ToList();

                        if (lst.Count() == 1)
                            selItem.Add((JFCExpendItem)lst.ElementAt(0));
                    }
                }

                if (grid.SelectedExpendItems.Count() == selItem.Count() && selItem.Any())
                {
                    var t = from item in grid.SelectedExpendItems
                            where !selItem.Contains(item)
                            select item;

                    if (t.Any())
                        grid.SelectedExpendItems = selItem;

                    //if (grid.SelectedExpendItems.Intersect(selItem).Count() == selItem.Count())
                    //    grid.SelectedExpendItems = selItem;

                }
                else if (selItem.Any() || grid.SelectedExpendItems.Any())
                    grid.SelectedExpendItems = selItem;

            }
        }

        private static void FindString(DependencyObject element, string stringFind)
        {

            int nbchildren = VisualTreeHelper.GetChildrenCount(element);

            for (int i = 0; i < nbchildren; i++)
            {
                DependencyObject el = VisualTreeHelper.GetChild(element, i);


            }

        }

        private static void UpdateSearchButtonVisibility(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGrid grid = obj as JFCGrid;

            if ((Boolean)e.NewValue == true)
                grid.SearchBorder.Visibility = Visibility.Visible;
            else
                grid.SearchBorder.Visibility = Visibility.Collapsed;
        }

        internal int NbRowAffichable()
        {
            if (this.VerticalAlignment == VerticalAlignment.Stretch)
            {
                if (!double.IsInfinity(MaxSize.Height))
                {
                    if (MaxSize.Height != 0)
                    {
                        int nbRowAff = 0;
                        double height = 0;

                        int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                        double HeightRowBefore = 0;
                        double HeightRowAfter = 0;

                        for (int i = 0; i < IdxRow; i++)
                        {
                            HeightRowBefore += TotalGrid.RowDefinitions[i].ActualHeight;
                        }

                        for (int i = IdxRow + 1; i < TotalGrid.RowDefinitions.Count; i++)
                        {
                            HeightRowAfter += TotalGrid.RowDefinitions[i].ActualHeight;
                        }

                        height = MaxSize.Height - HeightRowBefore - HeightRowAfter;

                        nbRowAff = (int)(height / RowHeight);

                        if ((height % RowHeight) != 0)
                        {
                            nbRowAff++;
                        }

                        if (nbRowAff < 0)
                            nbRowAff = 0;

                        return nbRowAff;
                    }
                    else
                    {
                        int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(1.0, GridUnitType.Star);

                        return 0;
                    }
                }
                else
                {
                    int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                    if (dataSource != null)
                    {
                        if (dataSource.Count() < NbRowMax)
                            TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(dataSource.Count() * RowHeight);
                        else
                            TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(NbRowMax * RowHeight);
                    }
                    else
                    {
                        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(0);
                    }

                    return NbRowMax;
                }
            }
            else
            {
                int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                if (dataSource != null)
                {
                    if (dataSource.Count() < NbRowMax)
                        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(dataSource.Count() * RowHeight);
                    else
                        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(NbRowMax * RowHeight);

                }
                else
                {
                    TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(0);
                }

                return NbRowMax;
            }
        }

        private int NbRowAffichableEntiere()
        {
            if (this.VerticalAlignment == VerticalAlignment.Stretch)
            {
                if (!double.IsInfinity(MaxSize.Height))
                {
                    if (MaxSize.Height != 0)
                    {
                        int nbRowAff = 0;
                        double height = 0;

                        int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                        double HeightRowBefore = 0;
                        double HeightRowAfter = 0;

                        for (int i = 0; i < IdxRow; i++)
                        {
                            HeightRowBefore += TotalGrid.RowDefinitions[i].ActualHeight;
                        }

                        for (int i = IdxRow + 1; i < TotalGrid.RowDefinitions.Count; i++)
                        {
                            HeightRowAfter += TotalGrid.RowDefinitions[i].ActualHeight;
                        }

                        height = MaxSize.Height - HeightRowBefore - HeightRowAfter;

                        nbRowAff = (int)(height / RowHeight);

                        return nbRowAff;
                    }
                    else
                    {
                        int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(1.0, GridUnitType.Star);

                        return 0;
                    }
                }
                else
                {
                    int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                    if (dataSource != null)
                    {
                        if (dataSource.Count() < NbRowMax)
                            TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(dataSource.Count() * RowHeight);
                        else
                            TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(NbRowMax * RowHeight);
                    }
                    else
                    {
                        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(0);
                    }

                    return NbRowMax;
                }
            }
            else
            {
                int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                if (dataSource != null)
                {
                    if (dataSource.Count() < NbRowMax)
                        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(dataSource.Count() * RowHeight);
                    else
                        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(NbRowMax * RowHeight);
                }
                else
                {
                    TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(0);
                }

                return NbRowMax;
            }
        }

        public int NbRowDisplayed
        {
            get
            {
                int nbRow = NbRowAffichableEntiere();

                if (Grid11.RowDefinitions.Count() < nbRow)
                    return Grid11.RowDefinitions.Count();
                else
                    return nbRow;
            }
        }

        internal int NbColumnAffichable()
        {
            if (this.HorizontalAlignment == System.Windows.HorizontalAlignment.Stretch)
            {
                if (!double.IsInfinity(MaxSize.Width))
                {
                    if (MaxSize.Width != 0)
                    {
                        int nbColAff = 0;
                        double width = 0;

                        int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                        double WidthColBefore = 0;
                        double WidthColAfter = 0;

                        for (int i = 0; i < IdxCol; i++)
                        {
                            WidthColBefore += TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }

                        for (int i = IdxCol + 1; i < TotalGrid.ColumnDefinitions.Count; i++)
                        {
                            WidthColAfter += TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }

                        width = MaxSize.Width - WidthColBefore - WidthColAfter;

                        double wcol = 0;

                        for (int i = (int)ScrollBarHorizontal.Value; i < Manager.NbColumn(JFCGridColumn.FrozenType.None) && wcol < width; i++)
                        {
                            wcol += Manager.ColumnWidth(JFCGridColumn.FrozenType.None, i);
                            nbColAff++;
                        }

                        if (nbColAff < 0)
                            nbColAff = 0;

                        return nbColAff;
                    }
                    else
                    {
                        int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength(1.0, GridUnitType.Star);

                        return 0;
                    }
                }
                else
                {
                    int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                    if (Manager.NbColumn(JFCGridColumn.FrozenType.None) > 0)
                    {
                        int NbCol = Manager.NbColumn(JFCGridColumn.FrozenType.None) - (int)ScrollBarHorizontal.Value;

                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength();

                        return NbCol;
                    }
                    else
                    {
                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength(0);
                        return 0;
                    }
                }
            }
            else
            {
                int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                //TODO
                //if (dataSource != null)
                //{
                //    if (dataSource.Count() < NbRowMax)
                //        TotalGrid.RowDefinitions[IdxCol].Height = new GridLength(dataSource.Count() * RowHeight);
                //    else
                //        TotalGrid.RowDefinitions[IdxCol].Height = new GridLength(NbRowMax * RowHeight);

                //}
                //else
                //{
                //    TotalGrid.RowDefinitions[IdxCol].Height = new GridLength(0);
                //}

                //return NbRowMax;
            }

            return 0;
        }

        private int NbColumnAffichableEntiere()
        {
            if (this.HorizontalAlignment == HorizontalAlignment.Stretch)
            {
                if (!double.IsInfinity(MaxSize.Width))
                {
                    if (MaxSize.Width != 0)
                    {
                        int nbColAff = 0;
                        double width = 0;

                        int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                        double WidthColumnBefore = 0;
                        double WidthColumnAfter = 0;

                        for (int i = 0; i < IdxCol; i++)
                        {
                            WidthColumnBefore += TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }

                        for (int i = IdxCol + 1; i < TotalGrid.ColumnDefinitions.Count; i++)
                        {
                            WidthColumnAfter += TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }

                        width = MaxSize.Width - WidthColumnBefore - WidthColumnAfter;

                        double wcol = 0;

                        for (int i = (int)ScrollBarHorizontal.Value; i < Manager.NbColumn(JFCGridColumn.FrozenType.None) && wcol < width; i++)
                        {
                            wcol += Manager.ColumnWidth(JFCGridColumn.FrozenType.None, i);
                            nbColAff++;
                        }

                        if (wcol > width)
                            nbColAff--;

                        if (nbColAff < 0)
                            nbColAff = 0;

                        return nbColAff;
                    }
                    else
                    {
                        int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength(1.0, GridUnitType.Star);

                        return 0;
                    }
                }
                else
                {
                    int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                    if (Manager.NbColumn(JFCGridColumn.FrozenType.None) > 0)
                    {
                        int NbCol = Manager.NbColumn(JFCGridColumn.FrozenType.None) - (int)ScrollBarHorizontal.Value;

                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength();

                        return NbCol;
                    }
                    else
                    {
                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength(0);
                        return 0;
                    }
                }
            }
            else
            {
                //TODO
                //int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                //if (dataSource != null)
                //{
                //    if (dataSource.Count() < NbRowMax)
                //        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(dataSource.Count() * RowHeight);
                //    else
                //        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(NbRowMax * RowHeight);
                //}
                //else
                //{
                //    TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(0);
                //}

                //return NbRowMax;
            }

            return 0;
        }

        private int NbColumnAffichableEntiereFin()
        {
            if (this.HorizontalAlignment == HorizontalAlignment.Stretch)
            {
                if (!double.IsInfinity(MaxSize.Width))
                {
                    if (MaxSize.Width != 0)
                    {
                        int nbColAff = 0;
                        double width = 0;

                        int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                        double WidthColumnBefore = 0;
                        double WidthColumnAfter = 0;

                        for (int i = 0; i < IdxCol; i++)
                        {
                            WidthColumnBefore += TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }

                        for (int i = IdxCol + 1; i < TotalGrid.ColumnDefinitions.Count; i++)
                        {
                            WidthColumnAfter += TotalGrid.ColumnDefinitions[i].ActualWidth;
                        }

                        width = MaxSize.Width - WidthColumnBefore - WidthColumnAfter;

                        double wcol = 0;

                        for (int i = Manager.NbColumn(JFCGridColumn.FrozenType.None) - 1; i >= 0 && wcol < width; i--)
                        {
                            wcol += Manager.ColumnWidth(JFCGridColumn.FrozenType.None, i);
                            nbColAff++;
                        }

                        if (wcol > width)
                            nbColAff--;

                        if (nbColAff < 0)
                            nbColAff = 0;

                        return nbColAff;
                    }
                    else
                    {
                        int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength(1.0, GridUnitType.Star);

                        return 0;
                    }
                }
                else
                {
                    int IdxCol = (int)ScrollViewerBody.GetValue(Grid.ColumnProperty);

                    if (Manager.NbColumn(JFCGridColumn.FrozenType.None) > 0)
                    {
                        int NbCol = Manager.NbColumn(JFCGridColumn.FrozenType.None) - (int)ScrollBarHorizontal.Value;

                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength();

                        return NbCol;
                    }
                    else
                    {
                        TotalGrid.ColumnDefinitions[IdxCol].Width = new GridLength(0);
                        return 0;
                    }
                }
            }
            else
            {
                //TODO
                //int IdxRow = (int)ScrollViewerBody.GetValue(Grid.RowProperty);

                //if (dataSource != null)
                //{
                //    if (dataSource.Count() < NbRowMax)
                //        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(dataSource.Count() * RowHeight);
                //    else
                //        TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(NbRowMax * RowHeight);
                //}
                //else
                //{
                //    TotalGrid.RowDefinitions[IdxRow].Height = new GridLength(0);
                //}

                //return NbRowMax;
            }

            return 0;
        }

        private void OnDataSourceChanged()
        {
            if (dataSource != null)
            {
                //dataSource
                int NbRowGridBody = Grid11.RowDefinitions.Count();
                int dataSourceCount = dataSource.Count();
                int NbRowAffi = NbRowAffichable();

                // on met a jour la selection
                foreach (var item in SelectedExpendItems.ToArray())
                {
                    if (!dataSource.Contains(item))
                    {
                        SelectedExpendItems.Remove(item);
                    }
                }

                //var lstRemove = SelectedExpendItems.Except(dataSource);
                //SelectedExpendItems.Remove;


                if (NbRowGridBody < NbRowAffi && NbRowGridBody < dataSourceCount)
                {
                    while (NbRowGridBody < NbRowAffi && NbRowGridBody < dataSourceCount)
                    {
                        Manager.RowAdd(false);

                        NbRowGridBody++;
                    }
                }

                //while (ScrollViewerBody.ScrollableHeight == 0)
                //{
                //    Manager.RowAdd(false);
                //}

                Manager.RowRefreshDataAll();
            }
            else
            {
                Manager.RowRemoveAll();
            }

        }

        private void OnDataSourceFooterChanged()
        {
            if (dataSourceFooter != null)
            {
                ////dataSource
                //int NbRowGridBody = GridBody.RowDefinitions.Count();
                //int dataSourceCount = dataSource.Count();
                //int NbRowAffi = NbRowAffichable();

                //// on met a jour la selection
                //foreach (var item in SelectedItems.ToArray())
                //{
                //    if (!dataSource.Contains(item))
                //    {
                //        SelectedItems.Remove(item);
                //    }
                //}

                //if (NbRowGridBody < NbRowAffi && NbRowGridBody < dataSourceCount)
                //{
                //    while (NbRowGridBody < NbRowAffi && NbRowGridBody < dataSourceCount)
                //    {
                //        Manager.RowAdd(false);

                //        NbRowGridBody++;
                //    }
                //}

                Manager.RowFooterRemoveAll();

                if (dataSourceFooter != null)
                {
                    for (int i = 0; i < dataSourceFooter.Count(); i++)
                        Manager.RowFooterAdd(true);
                }

                //Manager.RowFooterRefreshDataAll();
            }
            else
            {
                Manager.RowFooterRemoveAll();
            }

        }

        private void ScrollBarHorizontal_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (IsVirtualised == false)
            {
                int value = (int)ScrollBarHorizontal.Value;
                ScrollViewerBody.ScrollToHorizontalOffset(value);
                ScrollViewerBodyFooter.ScrollToHorizontalOffset(value);
                ScrollViewerHeaderColumn.ScrollToHorizontalOffset(value);
            }
            else
            {
                ScrollHorizontalValue = (int)e.NewValue;
            }
        }

        private void ScrollHorizontalValueChanged(int OldValue, int NewValue)
        {
            if (ColunmDefinition.Count() > 0)
            {
                if (isVirtualised == true)
                {
                    if (Manager.NbColumn(JFCGridColumn.FrozenType.None) > 0)
                    {
                        int NbColumn = NbColumnAffichable();
                        Manager.SetStartIndexAndNbCol(NewValue, NbColumn);

                    }
                    else
                    {
                        Manager.ColumnRemoveAll(false);
                    }
                }
                else
                {
                    Manager.NbColView = 0;
                    Manager.SetStartIndexAndNbCol(0, Int32.MaxValue);
                }

                RefreshScrollBarHorizontal(false);
            }
            else
            {
                Manager.ColumnRemoveAll();
            }
        }

        private void ScrollBarVertical_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            ScrollVerticalValue = (int)e.NewValue;
        }

        private void ScrollVerticalValueChanged(int OldValue, int NewValue)
        {
            int Delta = NewValue - OldValue;

            int NbRow = Grid11.RowDefinitions.Count();
            int NbRowAffi = NbRowAffichable();

            if (dataSource != null)
            {

                //if (Math.Abs(Delta) > NbRowAffi / 2)
                if (Math.Abs(Delta) > 4)
                {
                    if (scrollVerticalValue + NbRow > dataSource.Count())
                    {
                        // je supprime la ligne du bas quand j'affiche la derniere ligne
                        //Manager.RowRemove(NbRowAffi - 1);

                        for (int i = NbRow; i > (dataSource.Count() - scrollVerticalValue); i--)
                        {
                            Manager.RowRemove(i - 1);
                        }
                    }
                    else if (NbRow < NbRowAffi)
                    {
                        // j'ajoute la ligne en bas quand il en manque une
                        //Manager.RowAdd();
                        for (int i = NbRow; i < NbRowAffi; i++)
                        {
                            Manager.RowAdd(false);
                        }
                    }

                    // je mets a jour toutes les lignes
                    Manager.RowRefreshDataAll();

                }
                else
                {
                    if (scrollVerticalValue + NbRow > dataSource.Count())
                    {
                        // je supprime la ligne du haut quand j'affiche la derniere ligne
                        Manager.RowRemove(0);

                        // je mets a jour toutes les lignes
                        Manager.RowRefreshDataAll();
                    }
                    else
                    {
                        if (OldValue < NewValue)
                        {
                            Manager.RowMoveBeginToEnd(NewValue - OldValue);

                            if (NbRow < NbRowAffi)
                            {
                                // j'ajoute la ligne en bas quand il en manque une
                                Manager.RowAdd(true);
                            }
                        }
                        else if (OldValue > NewValue)
                        {
                            Manager.RowMoveEndToBegin(OldValue - NewValue);

                            if (NbRow < NbRowAffi)
                            {
                                // j'ajoute la ligne en bas quand il en manque une
                                Manager.RowAdd(true);
                            }
                        }
                    }
                }
            }
            else
            {
                Manager.RowRemoveAll();
            }
        }

        private void RefreshScrollBarHorizontal()
        {
            if (IsVirtualised == false)
            {
                Binding binding = new Binding("ScrollableWidth");
                binding.Source = ScrollViewerBody;
                binding.Mode = BindingMode.OneWay;

                this.ScrollBarHorizontal.SetBinding(ScrollBar.MaximumProperty, binding);

                Binding binding1 = new Binding("ViewportWidth");
                binding1.Source = ScrollViewerBody;
                binding1.Mode = BindingMode.OneWay;

                this.ScrollBarHorizontal.SetBinding(ScrollBar.ViewportSizeProperty, binding1);

                Binding binding2 = new Binding("ViewportWidth");
                binding2.Source = ScrollViewerBody;
                binding2.Mode = BindingMode.OneWay;
                this.ScrollBarHorizontal.SetBinding(ScrollBar.LargeChangeProperty, binding2);

                Binding binding3 = new Binding("ViewportWidth");
                binding3.Source = ScrollViewerBody;
                binding3.Mode = BindingMode.OneWay;
                this.ScrollBarHorizontal.SetBinding(ScrollBar.SmallChangeProperty, binding3);
            }
            else
            {
                RefreshScrollBarHorizontal(true);
            }
        }

        internal void RefreshScrollBarHorizontal(bool UpdateNbColumn)
        {
            if (IsVirtualised == true)
            {
                double MaxScroll = 0.0;

                int nbCol = Manager.NbColumn(JFCGridColumn.FrozenType.None);

                if (nbCol != 0)
                    MaxScroll = nbCol;

                if (MaxScroll <= 0 || double.IsNaN(MaxScroll))
                {
                    this.ScrollBarHorizontal.Minimum = 0;
                    this.ScrollBarHorizontal.Maximum = 0;
                    this.ScrollBarHorizontal.Value = 0;
                }
                else
                {
                    int NbColumnEntiere = NbColumnAffichableEntiere();
                    int NbColumnEntiereFin = NbColumnAffichableEntiereFin();

                    if (NbColumnEntiere == 0)
                        NbColumnEntiere = 1;

                    if (NbColumnEntiereFin == 0)
                        NbColumnEntiereFin = 1;

                    this.ScrollBarHorizontal.Minimum = 0;

                    //this.ScrollBarHorizontal.Maximum = MaxScroll - NbColumnEntiere;
                    this.ScrollBarHorizontal.Maximum = MaxScroll - NbColumnEntiereFin;

                    //this.ScrollBarHorizontal.ViewportSize = NbColumnEntiere;
                    this.ScrollBarHorizontal.ViewportSize = 1.0;
                    this.ScrollBarHorizontal.SmallChange = 1.0;

                    if (UpdateNbColumn)
                    {
                        if (IsVirtualised == true)
                        {
                            int NbColumn = NbColumnAffichable();
                            Manager.NbColView = NbColumn;
                        }
                        else
                        {
                            Manager.NbColView = Int32.MaxValue;
                        }
                    }

                    if (NbColumnEntiere > 0)
                        this.ScrollBarHorizontal.LargeChange = NbColumnEntiere - 1;

                }
            }

            RefreshScrollBarHorizontalVisibility();
        }

        private void RefreshScrollBarVertical()
        {
            double MaxScroll = 0.0;

            if (dataSource != null)
            {
                int NbRowEntiere = NbRowAffichableEntiere();
                MaxScroll = dataSource.Count() - NbRowEntiere;
            }

            if (MaxScroll <= 0 || double.IsNaN(MaxScroll))
            {
                this.ScrollBarVertical.Minimum = 0;
                this.ScrollBarVertical.Maximum = 0;
                this.ScrollBarVertical.Value = 0;
            }
            else
            {
                int NbRow = NbRowAffichable();

                if (this.ScrollBarVertical.Value > MaxScroll)
                    this.ScrollBarVertical.Value = MaxScroll;

                this.ScrollBarVertical.Minimum = 0;

                //if (NbRow >= dataSource.Count())
                this.ScrollBarVertical.Maximum = MaxScroll;
                //else
                //    this.ScrollBarVertical.Maximum = MaxScroll + 1;

                this.ScrollBarVertical.ViewportSize = NbRow;
                this.ScrollBarVertical.SmallChange = 1.0;

                if (NbRow > 0)
                    this.ScrollBarVertical.LargeChange = NbRow - 1;

            }

            RefreshScrollBarVerticalVisibility();
        }

        private void ScrollViewerBody_SizeChanged(object sender, SizeChangedEventArgs e)
        {

            Manager.RefreshSizeColumnStar();

            RefreshScrollBarVertical();
            RefreshScrollBarHorizontal();

            if (dataSource != null)
            {
                if (e.PreviousSize.Height < e.NewSize.Height)
                {
                    if (Grid11.RowDefinitions.Count() < dataSource.Count())
                    {
                        int NbRow = NbRowAffichable();
                        Boolean addDebut = false;
                        Boolean addedRow = false;

                        if (ScrollBarVertical.Value == ScrollBarVertical.Maximum)
                            addDebut = true;

                        for (int r = Grid11.RowDefinitions.Count(); r < NbRow; r++)
                        {
                            if (addDebut == false)
                                Manager.RowAdd(false);
                            else
                                // A REVOIR: car l'ajout de ligne au début ne marche pas tout le temps
                                //Manager.RowAdd(0);
                                Manager.RowAdd(false);

                            addedRow = true;
                        }

                        if (addedRow == true)
                            Manager.RowRefreshDataAll();
                    }
                }
                else
                {
                    int NbRow = NbRowAffichable();

                    for (int r = Grid11.RowDefinitions.Count() - 1; r > NbRow; r--)
                    {
                        Manager.RowRemove(r);
                    }
                }
            }

            RefreshScrollBarHorizontalVisibility();

        }

        private void ScrollViewer_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
        {
            int delta = 4;

            if (e.Delta > 0)
                ScrollBarVertical.Value = ScrollBarVertical.Value - delta;
            else
                ScrollBarVertical.Value = ScrollBarVertical.Value + delta;

            e.Handled = true;
        }

        public void ResetHeadersSorting()
        {
            ResetSorting(ColunmDefinition);
        }

        private void ResetSorting(ObservableCollection<JFCGridColumn> Columns)
        {
            foreach (var col in Columns)
            {
                col.Header.IsSort = JFCGridColumnHeader.SortType.None;

                ResetSorting(col.ChildrenColumns);
            }
        }

        private void ScrollViewer_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            int delta = 1;
            int deltaRepeat = 5;

            if (e.Key == Key.Down)
            {
                if (e.IsDown)
                {
                    if (e.IsRepeat == false)
                    {
                        ScrollBarVertical.Value = ScrollBarVertical.Value + delta;
                    }
                    else
                    {
                        ScrollBarVertical.Value = ScrollBarVertical.Value + deltaRepeat;
                    }
                }

                e.Handled = true;

            }
            else if (e.Key == Key.Up)
            {
                if (e.IsDown)
                {
                    if (e.IsRepeat == false)
                    {
                        ScrollBarVertical.Value = ScrollBarVertical.Value - delta;
                    }
                    else
                    {
                        ScrollBarVertical.Value = ScrollBarVertical.Value - deltaRepeat;
                    }
                }

                e.Handled = true;
            }
            else if (e.Key == Key.PageDown)
            {
                if (e.IsDown)
                {
                    //if (e.IsRepeat == false)
                    //{
                    //    ScrollBarVertical.Value = ScrollBarVertical.Value + delta;
                    //}
                    //else
                    //{
                    //    ScrollBarVertical.Value = ScrollBarVertical.Value + deltaRepeat;
                    //}

                    ScrollBarVertical.Value = ScrollBarVertical.Value + (Grid11.RowDefinitions.Count() - 1);
                }

                e.Handled = true;

            }
            else if (e.Key == Key.PageUp)
            {
                if (e.IsDown)
                {
                    //if (e.IsRepeat == false)
                    //{
                    //    ScrollBarVertical.Value = ScrollBarVertical.Value - delta;
                    //}
                    //else
                    //{
                    //    ScrollBarVertical.Value = ScrollBarVertical.Value - deltaRepeat;
                    //}

                    ScrollBarVertical.Value = ScrollBarVertical.Value - (Grid11.RowDefinitions.Count() - 1);
                }

                e.Handled = true;
            }
            else if ((Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl)) && Keyboard.IsKeyDown(Key.A))
            {
                Manager.SelectAll();

                e.Handled = true;
            }
            else if ((Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl)) && Keyboard.IsKeyDown(Key.Home))
            {
                ScrollBarVertical.Value = 0;

                e.Handled = true;
            }
            else if ((Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl)) && Keyboard.IsKeyDown(Key.End))
            {
                ScrollBarVertical.Value = ScrollBarVertical.Maximum;

                e.Handled = true;
            }
        }

        protected override Size MeasureOverride(Size constraint)
        {
            MaxSize = constraint;

            return base.MeasureOverride(constraint);
        }

        private void RefreshScrollBarHorizontalVisibility()
        {

            //if (ScrollBarHorizontalVisibility == ScrollBarVisibility.Visible)
            //{
            //    ScrollBarHorizontal.Visibility = Visibility.Visible;
            //}
            //else if (ScrollBarHorizontalVisibility == ScrollBarVisibility.Auto)
            //{
            //    if (ScrollViewerBody.ScrollableWidth != 0)
            //        ScrollBarHorizontal.Visibility = Visibility.Visible;
            //    else
            //        ScrollBarHorizontal.Visibility = Visibility.Hidden;
            //}
            //else if (ScrollBarHorizontalVisibility == ScrollBarVisibility.Hidden)
            //{
            //    ScrollBarHorizontal.Visibility = Visibility.Hidden;
            //}
            //else if (ScrollBarHorizontalVisibility == ScrollBarVisibility.Disabled)
            //{

            //}
        }

        private void RefreshScrollBarVerticalVisibility()
        {
            //if (ScrollBarVerticalVisibility == ScrollBarVisibility.Visible)
            //{
            //    ScrollBarVertical.Visibility = Visibility.Visible;
            //}
            //else if (ScrollBarVerticalVisibility == ScrollBarVisibility.Auto)
            //{
            //    //if (ScrollViewerBody.ScrollableHeight != 0)
            //    //    ScrollBarVertical.Visibility = Visibility.Visible;
            //    //else
            //    //    ScrollBarVertical.Visibility = Visibility.Collapsed;

            //    ScrollBarVertical.Visibility = ScrollViewerBody.ComputedHorizontalScrollBarVisibility;
            //}
            //else if (ScrollBarVerticalVisibility == ScrollBarVisibility.Hidden)
            //{
            //    ScrollBarVertical.Visibility = Visibility.Hidden;
            //}
            //else if (ScrollBarVerticalVisibility == ScrollBarVisibility.Disabled)
            //{

            //}



            //ScrollViewerBody.CanContentScroll = false;

        }

        private void ScrollViewerHeaderRow_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            if (e.VerticalChange > 0)
                ScrollViewerHeaderRow.ScrollToTop();
        }

        private void ScrollViewerHeaderColumn_ScrollChanged(object sender, ScrollChangedEventArgs e)
        {
            if (IsVirtualised == false)
            {
                if (e.HorizontalOffset != ScrollBarHorizontal.Value)
                    ScrollViewerHeaderColumn.ScrollToHorizontalOffset(ScrollBarHorizontal.Value);
            }
            else
            {
                if (e.HorizontalChange > 0)
                    ScrollViewerHeaderColumn.ScrollToHorizontalOffset(0);
            }
        }

        IInputElement beforeFocused = null;
        private bool _IsSearchVisible = false;
        public bool IsSearchVisible
        {
            get { return _IsSearchVisible; }
            set
            {
                _IsSearchVisible = value;

                if (value == true)
                {
                    RowSearch.Height = new GridLength();
                    beforeFocused = Keyboard.FocusedElement;

                    WrapPanelSearch.Visibility = System.Windows.Visibility.Visible;

                    TextBoxSearch.Focus();

                    Manager.RowRefreshDataAll();
                }
                else
                {
                    RowSearch.Height = new GridLength(0);

                    if (beforeFocused != null)
                        beforeFocused.Focus();

                    WrapPanelSearch.Visibility = System.Windows.Visibility.Collapsed;

                    Manager.RowRefreshDataAll();
                }
            }
        }

        private void TextBoxSearch_TextChanged(object sender, TextChangedEventArgs e)
        {
            OnSearchChanged(this, new DependencyPropertySearchChangedEventArgs(TextBoxSearch.Text, SearchResult));
        }

        private void TextBoxSearch_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Return)
                SearchNext_Click(sender, new RoutedEventArgs());
        }

        private void SearchClose_Click(object sender, RoutedEventArgs e)
        {
            //RowSearch.Height = new GridLength(0);
            IsSearchVisible = false;
        }

        //private bool FindObject(object item, object element, ref int index, List<int> lstExpand)
        //{
        //    PropertyInfo d = item.GetType().GetProperty(PropertyChildrenName);

        //    if (d != null)
        //    {
        //        if (d.PropertyType.IsArray)
        //        {
        //            Object[] Tab = (Object[])d.GetValue(item, null);

        //            foreach (var elmt in Tab)
        //            {
        //                if (elmt == element)
        //                {
        //                    //ScrollTo(index);
        //                    lstExpand.Add(index);

        //                    return true;
        //                }

        //                index++;

        //                if (PropertyChildrenName != "")
        //                {
        //                    bool result = FindObject(elmt, element, ref index, lstExpand);

        //                    if (result == true)
        //                    {
        //                        // TODO expand

        //                        return result;
        //                    }
        //                }
        //            }
        //        }
        //        else
        //        {
        //            object o = d.GetValue(item, null);

        //            if (o is IEnumerable)
        //            {
        //                IEnumerable ienum = (IEnumerable)d.GetValue(item, null);

        //                foreach (var elmt in ienum)
        //                {
        //                    if (elmt == element)
        //                    {
        //                        //ScrollTo(index);
        //                        lstExpand.Add(index);

        //                        return true;
        //                    }

        //                    index++;

        //                    if (PropertyChildrenName != "")
        //                    {
        //                        int indextmp = index;

        //                        bool result = FindObject(elmt, element, ref indextmp, lstExpand);

        //                        if (result == true)
        //                        {
        //                            lstExpand.Add(index - 1);
        //                            index = indextmp;
        //                            return result;
        //                        }
        //                    }
        //                }
        //            }
        //        }
        //    }

        //    return false;
        //}

        private Boolean FlagSearchObject(IEnumerable<JFCExpendItem> lstds)
        {

            //int level = 0;
            //int index = -1;
            //bool finded = false;

            //index = 0;

            SearchResult.CollectionChanged -= SearchResult_CollectionChanged;

            for (int i = 0; i < SearchResult.Count(); i++)
            {
                var pos = SearchResult[i];

                var elm = pos.Path.Last();
                int level = pos.Path.Count() - 1;

                var lst = from item in lstds
                          where item.Obj == elm && item.Level == level
                          select item;

                foreach (var item in lst)
                {
                    if (item.Level == 0)
                    {
                        pos.Item = item;
                        SearchResult[i] = pos;

                        break;
                    }
                    else
                    {
                        if (item.Parent.Obj == pos.Path.ElementAt(pos.Path.Count() - 2))
                        {
                            pos.Item = item;
                            SearchResult[i] = pos;

                            break;
                        }
                    }
                }


                //foreach (var item in lstds)
                //{
                //    if (item.Obj == elm)
                //    {
                //        if (item.Level == pos.Path.Count() - 1)
                //        {
                //            if (item.Level == 0)
                //            {
                //                pos.Item = item;
                //                SearchResult[i] = pos;

                //                break;
                //            }
                //            else
                //            {
                //                if (item.Parent.Obj == pos.Path.ElementAt(pos.Path.Count() - 2))
                //                {
                //                    pos.Item = item;
                //                    SearchResult[i] = pos;

                //                    break;
                //                }
                //            }
                //        }
                //    }
                //}
            }

            SearchResult.CollectionChanged += new NotifyCollectionChangedEventHandler(SearchResult_CollectionChanged);

            return false;

        }

        public Boolean ScrollVerticalTo(IEnumerable<object> lstElement) //, bool onlyVisible = false
        {

            int level = 0;
            int index = -1;
            bool finded = false;

            //var lstd = dataSource.ToList();
            index = 0;

            foreach (var elm in lstElement)
            {
                //var lstd = dataSource.ToList();

                //index = 0;
                //foreach (var item in lstd)
                //for (; index < lstd.Count(); index++)
                for (; index < dataSource.Count(); index++)
                {
                    //JFCExpendItem item = lstd[index];
                    JFCExpendItem item = dataSource[index];

                    if (item.Obj == elm)
                    {
                        if (item.Level == level)
                        {
                            if (elm != lstElement.Last())
                            {
                                if (item.IsExpended == false)
                                {
                                    //item.PropertyChanged -= item_PropertyChanged;

                                    item.IsExpended = true;

                                    //PropertyInfo d = item.Obj.GetType().GetProperty(PropertyChildrenName);

                                    //ExpandeItem(lstd, item, index, d);

                                    //item.PropertyChanged += new PropertyChangedExtendedEventHandler<object>(item_PropertyChanged);
                                }
                            }

                            finded = true;
                            break;
                        }
                    }

                    //index++;
                }

                if (finded == false)
                {
                    return false;
                }

                level++;
            }

            if (finded == true)
            {
                //dataSource = lstd.ToArray();

                Manager.RowRefreshDataAll();
                //OnDataSourceChanged();
                RefreshScrollBarVertical();

                return ScrollVerticalTo(index);
            }

            return false;

        }

        public Boolean ScrollVerticalTo(int index)
        {
            //ScrollVerticalValue = index;

            int nbRow = dataSource.Count();
            int nbAffi = NbRowAffichableEntiere();

            if (index <= (nbRow - nbAffi))
            {
                ScrollBarVertical.Value = index;
            }
            else
            {
                ScrollBarVertical.Value = ScrollBarVertical.Maximum; // -ScrollBarVertical.ViewportSize;
                return false;
            }

            return true;
        }

        public Boolean ScrollHorizontalTo(int index)
        {
            int nbCol = Manager.NbColumn(JFCGridColumn.FrozenType.None);

            if (index < nbCol)
            {
                ScrollBarHorizontal.Value = index;
            }
            else
            {
                ScrollBarHorizontal.Value = ScrollBarVertical.Maximum;
                return false;
            }

            return true;
        }

        public void RefreshData()
        {
            Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(delegate
            {
                this.Manager.RowRefreshDataAll();
                this.Manager.RowFooterRefreshDataAll();

            }));
        }

        private void SearchPrevious_Click(object sender, RoutedEventArgs e)
        {
            if (SearchResult.Count() > 0)
            {
                if (IndexSearch >= 1)
                {
                    IndexSearch--;
                }
                else
                {
                    IndexSearch = SearchResult.Count() - 1;
                }

                var position = SearchResult[IndexSearch];

                ScrollVerticalTo(position.Path);

                if (IndexSearch == -1)
                    TextBlockNbFind.Text = "0 / 0";
                else
                    TextBlockNbFind.Text = (IndexSearch + 1).ToString() + " / " + SearchResult.Count().ToString();

                //FlagSearchObject();
            }
        }

        private void SearchNext_Click(object sender, RoutedEventArgs e)
        {
            if (SearchResult.Count() > 0)
            {
                if (IndexSearch < SearchResult.Count() - 1)
                {
                    IndexSearch++;
                }
                else
                {
                    IndexSearch = 0;
                }

                var position = SearchResult[IndexSearch];

                ScrollVerticalTo(position.Path);

                if (IndexSearch == -1)
                    TextBlockNbFind.Text = "0 / 0";
                else
                    TextBlockNbFind.Text = (IndexSearch + 1).ToString() + " / " + SearchResult.Count().ToString();

                //FlagSearchObject();
            }
        }

        private void SearchButton_Click(object sender, RoutedEventArgs e)
        {
            if (IsSearchVisible)
                IsSearchVisible = false;
            else
                IsSearchVisible = true;
        }
    }
}

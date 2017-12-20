using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Media;
using System.Windows.Media;
using System.Windows.Data;
using System.Windows.Shapes;
using System.Collections;
using System.Windows.Input;
using System.Reflection;
using System.ComponentModel;
using System.Windows.Threading;
using System.Windows.Markup;
using System.IO;
using System.Xml;
using System.Collections.Specialized;

namespace JFCGridControl
{
    class JFCGridManager
    {
        //private List<JFCGridColumn> ListColBody;
        private ObservableCollection<JFCGridColumn> ListColBody;

        private List<JFCGridColumn> ListColHeaderRow;
        private List<JFCGridColumn> ListColHeaderFooter;

        private List<JFCGridColumn> ListColHeaderRowEnd;

        private Grid Grid00;
        private Grid Grid01;
        private Grid Grid02;

        private Grid Grid10;
        private Grid Grid11;
        private Grid Grid12;

        private Grid Grid20;
        private Grid Grid21;

        internal JFCGrid Parent;

        private Rectangle HeaderMove = new Rectangle();
        private Point PtHeaderMove;
        //private Rectangle HeaderMoveFlag = new Rectangle();
        private Rectangle ColDestinationIndicator;

        private int MaxLevelColumn = 0;

        //private List<List<UIElement>> TabLineH = new List<List<UIElement>>();
        //private List<List<UIElement>> TabLineHF = new List<List<UIElement>>();
        //private List<List<UIElement>> TabLineB = new List<List<UIElement>>();        
        //private List<List<UIElement>> TabLineBF = new List<List<UIElement>>();

        internal List<LinkedList<UIElement>> TabLineH = new List<LinkedList<UIElement>>();
        internal List<LinkedList<UIElement>> TabLineHF = new List<LinkedList<UIElement>>();
        internal List<LinkedList<UIElement>> TabLineB = new List<LinkedList<UIElement>>();
        internal List<LinkedList<UIElement>> TabLineBF = new List<LinkedList<UIElement>>();
        internal List<LinkedList<UIElement>> TabLineF = new List<LinkedList<UIElement>>();

        //private List<JFCGridCellGrouping> TabCellGrouping = new List<JFCGridCellGrouping>();

        private List<CellImg> LstRecycleCellImg = new List<CellImg>();
        private List<JFCGridCell> LstRecycleJFCGridCell = new List<JFCGridCell>();

        private List<CellImg> LstRemoveColCellImg = new List<CellImg>();
        private List<JFCGridCell> LstRemoveColJFCGridCell = new List<JFCGridCell>();

        private List<UIElement> LstRemoveHeader = new List<UIElement>();
        private List<UIElement> LstRemoveBelowHeader = new List<UIElement>();
        private List<UIElement> LstRemoveSplitter = new List<UIElement>();

        DispatcherTimer timer;

        private bool InProcess = false;

        private bool UseLstRemoveCol = true;

        private int startIndexColView = 0;
        public int StartIndexColView
        {
            get { return startIndexColView; }
            set
            {
                if (!InProcess)
                {
                    InProcess = true;

                    if (startIndexColView != value)
                    {
                        int oldValue = startIndexColView;
                        int newValue = value;

                        startIndexColView = value;

                        if (NbColView > 0)
                        {
                            if (oldValue < newValue)
                            {

                                // on supprime les colonnes du début
                                for (int i = oldValue; i < newValue; i++)
                                {
                                    if (Grid11.ColumnDefinitions.Count() > 0)
                                    {
                                        ColumnRemove(0, Grid11, TabLineB, UseLstRemoveCol);
                                        ColumnRemove(0, Grid21, TabLineBF);

                                        ColumnRemoveGridHeader(Grid01, 0);
                                    }
                                }

                                //ClearParentLstRemoveCol();

                                // on ajoute les colonnes à la fin
                                for (int i = Grid11.ColumnDefinitions.Count(); i < NbColView; i++)
                                {
                                    if (newValue + i < ListColBody.Count())
                                    {
                                        JFCGridColumn Column = ListColBody[newValue + i];

                                        ColumnAddGrid(Column, i, Grid11, TabLineB, ListColBody);
                                        ColumnAddGridFooter(Column, i, Grid21, TabLineBF, ListColBody);

                                        ColumnAddGridHeader(Grid01, Column, i);

                                        if (Column.Width.IsAuto == true)
                                        {
                                            if (Column.ActualWidth.IsAuto)
                                                Column.ActualWidth = CalculTailleMaxi(Column);
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }

                                ClearLstRemoveCol();

                            }
                            else if (oldValue > newValue)
                            {
                                int nbcol = Grid11.ColumnDefinitions.Count();

                                if (oldValue + nbcol != newValue + nbColView)
                                {
                                    if (oldValue + nbcol > newValue + nbColView)
                                    {
                                        int nbColRemove = (oldValue + nbcol) - (newValue + nbColView);

                                        for (int i = 0; i < nbColRemove; i++, nbcol--)
                                        {
                                            if (Grid11.ColumnDefinitions.Count() > 0)
                                            {
                                                ColumnRemove(nbcol - 1, Grid11, TabLineB, UseLstRemoveCol);
                                                ColumnRemove(nbcol - 1, Grid21, TabLineBF);

                                                ColumnRemoveGridHeader(Grid01, nbcol - 1);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        for (int i = nbcol; i > 0; i++, nbcol--)
                                        {
                                            if (Grid11.ColumnDefinitions.Count() > 0)
                                            {
                                                ColumnRemove(nbcol - 1, Grid11, TabLineB, UseLstRemoveCol);
                                                ColumnRemove(nbcol - 1, Grid21, TabLineBF);

                                                ColumnRemoveGridHeader(Grid01, nbcol - 1);
                                            }
                                        }
                                    }


                                    //// on supprime les colonnes à la fin
                                    ////for (int i = oldValue; i > newValue; i--, nbcol--)
                                    //for (int i = Grid11.ColumnDefinitions.Count(); i >= 0; i--, nbcol--)
                                    //{
                                    //    if (Grid11.ColumnDefinitions.Count() > 0)
                                    //    {
                                    //        ColumnRemove(nbcol - 1, Grid11, TabLineB, false);
                                    //        ColumnRemove(nbcol - 1, Grid21, TabLineBF);

                                    //        ColumnRemoveGridHeader(Grid01, nbcol - 1);
                                    //    }
                                    //}

                                    //ClearParentLstRemoveCol();

                                    nbcol = Grid11.ColumnDefinitions.Count();

                                    for (int i = 0; i < NbColView - nbcol; i++)
                                    {
                                        if (newValue + i < ListColBody.Count())
                                        {
                                            JFCGridColumn Column = ListColBody[newValue + i];

                                            ColumnAddGrid(Column, i, Grid11, TabLineB, ListColBody);
                                            ColumnAddGridFooter(Column, i, Grid21, TabLineBF, ListColBody);

                                            ColumnAddGridHeader(Grid01, Column, i);

                                            if (Column.Width.IsAuto == true)
                                            {
                                                if (Column.ActualWidth.IsAuto)
                                                    if (Column.ActualWidth.IsAuto)
                                                        Column.ActualWidth = CalculTailleMaxi(Column);
                                            }
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }

                                    ClearLstRemoveCol();

                                }
                                else
                                {
                                    nbcol = Grid11.ColumnDefinitions.Count();

                                    for (int i = 0; i < NbColView - nbcol; i++)
                                    {
                                        if (newValue + i < ListColBody.Count())
                                        {
                                            JFCGridColumn Column = ListColBody[newValue + i];

                                            ColumnAddGrid(Column, i, Grid11, TabLineB, ListColBody);
                                            ColumnAddGridFooter(Column, i, Grid21, TabLineBF, ListColBody);

                                            ColumnAddGridHeader(Grid01, Column, i);

                                            if (Column.Width.IsAuto == true)
                                            {
                                                if (Column.ActualWidth.IsAuto)
                                                    if (Column.ActualWidth.IsAuto)
                                                        Column.ActualWidth = CalculTailleMaxi(Column);
                                            }
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
                    }

                    InProcess = false;
                }
                else
                {
                    int i = 0;
                }

                if (InProcess)
                    throw new Exception();
            }
        }

        //public int endIndexColView = int.MaxValue - 1;
        private int nbColView = 0; //int.MaxValue;
        public int NbColView
        {
            get { return nbColView; }
            set
            {
                if (!InProcess)
                {
                    InProcess = true;

                    if (value == 0)
                    {
                        for (int i = Grid11.ColumnDefinitions.Count() - 1; i >= 0; i--)
                        {
                            ColumnRemove(i, Grid11, TabLineB);
                            ColumnRemove(i, Grid21, TabLineBF);

                            ColumnRemoveGridHeader(Grid01, i);
                        }

                        nbColView = value;
                    }
                    else if (nbColView != value)
                    {
                        int oldValue = nbColView;
                        int newValue = value;
                        nbColView = value;

                        if (newValue > oldValue)
                        {
                            int index = oldValue;
                            for (int i = oldValue; i < newValue; i++)
                            {
                                if (StartIndexColView + i < ListColBody.Count())
                                {
                                    JFCGridColumn Column = ListColBody[StartIndexColView + i];
                                    //ColumnAdd(col);

                                    ColumnAddGrid(Column, index, Grid11, TabLineB, ListColBody);
                                    ColumnAddGridFooter(Column, index, Grid21, TabLineBF, ListColBody);

                                    ColumnAddGridHeader(Grid01, Column, index);

                                    if (Column.Width.IsAuto == true)
                                    {
                                        if (Column.ActualWidth.IsAuto)
                                            Column.ActualWidth = CalculTailleMaxi(Column);
                                    }

                                    index++;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        else if (newValue < oldValue)
                        {
                            //for (int i = oldValue - 1; i >= newValue; i--)
                            for (int i = Grid11.ColumnDefinitions.Count() - 1; i >= newValue; i--)
                            {
                                if (StartIndexColView + i < ListColBody.Count())
                                {
                                    //JFCGridColumn col = ListColBody[StartIndexColView + i];
                                    //ColumnRemove(col);

                                    if (i < Grid11.ColumnDefinitions.Count())
                                    {
                                        ColumnRemove(i, Grid11, TabLineB);
                                        ColumnRemove(i, Grid21, TabLineBF);

                                        ColumnRemoveGridHeader(Grid01, i);
                                    }
                                }
                            }
                        }

                        ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
                    }
                    else if (Grid11.ColumnDefinitions.Count() != NbColView)
                    {
                        if (NbColView > Grid11.ColumnDefinitions.Count())
                        {
                            //int index = oldValue;
                            for (int i = Grid11.ColumnDefinitions.Count(); i < NbColView; i++)
                            {
                                if (StartIndexColView + i < ListColBody.Count())
                                {
                                    JFCGridColumn Column = ListColBody[StartIndexColView + i];
                                    //ColumnAdd(col);

                                    ColumnAddGrid(Column, i, Grid11, TabLineB, ListColBody);
                                    ColumnAddGridFooter(Column, i, Grid21, TabLineBF, ListColBody);

                                    ColumnAddGridHeader(Grid01, Column, i);

                                    if (Column.Width.IsAuto == true)
                                    {
                                        if (Column.ActualWidth.IsAuto)
                                            Column.ActualWidth = CalculTailleMaxi(Column);
                                    }

                                    //index++;
                                }
                                else
                                {
                                    break;
                                }
                            }
                        }
                        else if (NbColView < Grid11.ColumnDefinitions.Count())
                        {
                            for (int i = Grid11.ColumnDefinitions.Count(); i > NbColView; i--)
                            {
                                if (StartIndexColView + i < ListColBody.Count())
                                {
                                    JFCGridColumn col = ListColBody[StartIndexColView + i - 1];
                                    //ColumnRemove(col);

                                    ColumnRemove(i - 1, Grid11, TabLineB);
                                    ColumnRemove(i - 1, Grid21, TabLineBF);

                                    ColumnRemoveGridHeader(Grid01, i - 1);
                                }
                            }
                        }

                        ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
                    }

                    InProcess = false;
                }
                else
                {
                    int i = 0;
                }

                if (InProcess)
                    throw new Exception();
            }
        }

        public void SetStartIndexAndNbCol(int IdxStart, int NbCol)
        {
            //if (startIndexColView != IdxStart && nbColView != NbCol)
            //{
            //    if ()
            //    {

            //    }
            //    else
            //    {
            //    //ColumnRemoveAll(false);
            //    NbColView = 0;

            //    startIndexColView = IdxStart;
            //    NbColView = NbCol;
            //    }
            //}
            //else 

            if (startIndexColView != IdxStart)
            {
                nbColView = NbCol;
                StartIndexColView = IdxStart;
            }
            else if (nbColView != NbCol)
            {
                startIndexColView = IdxStart;
                NbColView = NbCol;
            }
        }

        public JFCGridManager(JFCGrid Parent)
        {
            this.Grid00 = Parent.Grid00;
            this.Grid01 = Parent.Grid01;
            this.Grid02 = Parent.Grid02;

            this.Grid10 = Parent.Grid10;
            this.Grid11 = Parent.Grid11;
            this.Grid12 = Parent.Grid12;

            this.Grid20 = Parent.Grid20;
            this.Grid21 = Parent.Grid21;

            this.Parent = Parent;

            //ListColBody = new List<JFCGridColumn>();
            ListColBody = new ObservableCollection<JFCGridColumn>();
            ListColBody.CollectionChanged += new System.Collections.Specialized.NotifyCollectionChangedEventHandler(ListColBody_CollectionChanged);

            ListColHeaderRow = new List<JFCGridColumn>();
            ListColHeaderFooter = new List<JFCGridColumn>();
            ListColHeaderRowEnd = new List<JFCGridColumn>();

            ColDestinationIndicator = new Rectangle();
            ColDestinationIndicator.HorizontalAlignment = HorizontalAlignment.Left;
            ColDestinationIndicator.VerticalAlignment = VerticalAlignment.Stretch;
            ColDestinationIndicator.Width = 5;
            ColDestinationIndicator.Fill = Brushes.Red;

            timer = new DispatcherTimer();
        }

        void ListColBody_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (Parent.IsVirtualised)
            {
                if (e.Action == NotifyCollectionChangedAction.Add)
                {
                    if (NbColView == 0)
                    {
                        foreach (JFCGridColumn Column in e.NewItems)
                        {
                            if (Column.Width.IsAuto == true)
                            {
                                if (Column.ActualWidth.IsAuto)
                                    Column.ActualWidth = CalculTailleMaxi(Column);
                            }

                            //Column.Item.ParentGrid = Parent;
                        }
                        return;
                    }

                    int i = 0;

                    foreach (JFCGridColumn Column in e.NewItems)
                    {
                        int index = e.NewStartingIndex + i;

                        if (StartIndexColView <= index && index <= (StartIndexColView + NbColView - 1))
                        {
                            if ((StartIndexColView + NbColView) < ListColBody.Count())
                            {
                                ///int idx = StartIndexColView + NbColView - 1;
                                int idx = StartIndexColView + Grid11.ColumnDefinitions.Count() - 1;

                                if (idx < Grid11.ColumnDefinitions.Count())
                                {
                                    ColumnRemove(idx, Grid11, TabLineB, UseLstRemoveCol);
                                    ColumnRemove(idx, Grid21, TabLineBF);
                                    ColumnRemoveGridHeader(Grid01, idx);
                                }

                                ClearParentLstRemoveCol();
                            }

                            ColumnAddGrid(Column, index - StartIndexColView, Grid11, TabLineB, ListColBody);
                            ColumnAddGridFooter(Column, index - StartIndexColView, Grid21, TabLineBF, ListColBody);
                            ColumnAddGridHeader(Grid01, Column, index - StartIndexColView);

                            //Column.Item.ParentGrid = Parent;

                            if (Column.Width.IsAuto == true)
                            {
                                if (Column.ActualWidth.IsAuto)
                                    Column.ActualWidth = CalculTailleMaxi(Column);
                            }
                        }

                        i++;
                    }

                    ClearLstRemoveCol();

                }
                else if (e.Action == NotifyCollectionChangedAction.Move)
                {
                    // on supprime la colonne à l'ancien emplacement
                    if (StartIndexColView <= e.OldStartingIndex && e.OldStartingIndex <= StartIndexColView + NbColView - 1)
                    {
                        int index = e.OldStartingIndex - StartIndexColView;

                        ColumnRemove(index, Grid11, TabLineB, UseLstRemoveCol);
                        ColumnRemove(index, Grid21, TabLineBF);
                        ColumnRemoveGridHeader(Grid01, index);
                    }

                    ClearParentLstRemoveCol();

                    // on ajoute la colonne au bonne endroit
                    if (StartIndexColView <= e.NewStartingIndex && e.NewStartingIndex <= StartIndexColView + NbColView - 1)
                    {
                        JFCGridColumn col = ListColBody[e.NewStartingIndex];

                        int index = e.NewStartingIndex - StartIndexColView;

                        ColumnAddGrid(col, index, Grid11, TabLineB, ListColBody);
                        ColumnAddGridFooter(col, index, Grid21, TabLineBF, ListColBody);
                        ColumnAddGridHeader(Grid01, col, index);
                    }

                    ClearLstRemoveCol();

                }
                else if (e.Action == NotifyCollectionChangedAction.Remove)
                {
                    if (NbColView == 0)
                        return;

                    int i = 0;
                    foreach (JFCGridColumn Column in e.OldItems)
                    {
                        int index = e.OldStartingIndex + i;

                        if (StartIndexColView <= index && index <= StartIndexColView + NbColView - 1)
                        {
                            index = index - StartIndexColView;

                            ColumnRemove(index, Grid11, TabLineB, UseLstRemoveCol);
                            ColumnRemove(index, Grid21, TabLineBF);
                            ColumnRemoveGridHeader(Grid01, index);

                            ClearParentLstRemoveCol();

                            if (StartIndexColView + NbColView - 1 < ListColBody.Count())
                            {
                                //int idx = EndIndexColView + 1 - StartIndexColView;

                                //index = StartIndexColView + NbColView - 1;

                                JFCGridColumn col = ListColBody[StartIndexColView + NbColView - 1];

                                index = NbColView - 1;

                                ColumnAddGrid(col, index, Grid11, TabLineB, ListColBody);
                                ColumnAddGridFooter(col, index, Grid21, TabLineBF, ListColBody);
                                ColumnAddGridHeader(Grid01, col, index);

                                if (Column.Width.IsAuto == true)
                                {
                                    if (Column.ActualWidth.IsAuto)
                                        Column.ActualWidth = CalculTailleMaxi(Column);
                                }
                            }
                        }

                        i++;
                    }
                }
                else if (e.Action == NotifyCollectionChangedAction.Replace)
                { }
                else if (e.Action == NotifyCollectionChangedAction.Reset)
                {
                    if (NbColView == 0)
                        return;

                    for (int index = Grid11.ColumnDefinitions.Count(); index > 0; index--)
                    {
                        ColumnRemove(0, Grid11, TabLineB);
                        ColumnRemove(0, Grid21, TabLineBF);
                        ColumnRemoveGridHeader(Grid01, 0);
                    }

                    startIndexColView = 0;

                    if (Parent.IsVirtualised == true)
                        nbColView = 0;

                    // on replace le scroll au début
                    Parent.ScrollBarVertical.Value = 0;
                }

                //startIndexColView = 0;
                //nbColView = 0;

                // on replace le scroll au début
                //Parent.ScrollBarVertical.Value = 0;

                ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
            }
            else
            {
                if (e.Action == NotifyCollectionChangedAction.Add)
                {


                    int i = e.NewStartingIndex;

                    foreach (JFCGridColumn Column in e.NewItems)
                    {

                        ColumnAddGrid(Column, i, Grid11, TabLineB, ListColBody);
                        ColumnAddGridFooter(Column, i, Grid21, TabLineBF, ListColBody);
                        ColumnAddGridHeader(Grid01, Column, i);

                        if (Column.Width.IsAuto == true)
                        {
                            if (Column.ActualWidth.IsAuto)
                                Column.ActualWidth = CalculTailleMaxi(Column);
                        }
                        else if (Column.Width.IsStar == true)
                        {

                            //GridLength wCal = CalculTailleMaxi(Column);

                            //if (wCal.Value < Parent.ScrollViewerBody.ViewportWidth)
                            //{
                            //    Column.ActualWidth = new GridLength(Parent.ScrollViewerBody.ViewportWidth);
                            //}
                            //else
                            //{
                            //    Column.ActualWidth = wCal;
                            //}

                            //Parent.ScrollViewerBody.SizeChanged += delegate(object sender1, SizeChangedEventArgs e1)
                            //{
                            //    if (e1.WidthChanged)
                            //    {
                            //        GridLength wCal1 = CalculTailleMaxi(Column);

                            //        if (wCal1.Value < Parent.ScrollViewerBody.ViewportWidth)
                            //        {
                            //            Column.ActualWidth = new GridLength(Parent.ScrollViewerBody.ViewportWidth);
                            //        }
                            //        else
                            //        {
                            //            Column.ActualWidth = wCal1;
                            //        }
                            //    }
                            //};
                        }

                        i++;
                    }
                }
                else if (e.Action == NotifyCollectionChangedAction.Move)
                {
                    // on supprime la colonne à l'ancien emplacement
                    int index = e.OldStartingIndex;

                    ColumnRemove(index, Grid11, TabLineB, UseLstRemoveCol);
                    ColumnRemove(index, Grid21, TabLineBF);
                    ColumnRemoveGridHeader(Grid01, index);

                    ClearParentLstRemoveCol();

                    // on ajoute la colonne au bonne endroit
                    JFCGridColumn col = ListColBody[e.NewStartingIndex];

                    index = e.NewStartingIndex;

                    ColumnAddGrid(col, index, Grid11, TabLineB, ListColBody);
                    ColumnAddGridFooter(col, index, Grid21, TabLineBF, ListColBody);
                    ColumnAddGridHeader(Grid01, col, index);

                    ClearLstRemoveCol();
                }
                else if (e.Action == NotifyCollectionChangedAction.Remove)
                {
                    int i = 0;
                    foreach (JFCGridColumn Column in e.OldItems)
                    {
                        int index = e.OldStartingIndex + i;

                        ColumnRemove(index, Grid11, TabLineB);
                        ColumnRemove(index, Grid21, TabLineBF);
                        ColumnRemoveGridHeader(Grid01, index);

                        i++;
                    }
                }
                else if (e.Action == NotifyCollectionChangedAction.Replace)
                { }
                else if (e.Action == NotifyCollectionChangedAction.Reset)
                {
                    for (int index = Grid11.ColumnDefinitions.Count(); index > 0; index--)
                    {
                        ColumnRemove(0, Grid11, TabLineB);
                        ColumnRemove(0, Grid21, TabLineBF);
                        ColumnRemoveGridHeader(Grid01, 0);
                    }
                }

                startIndexColView = 0;

                if (Parent.IsVirtualised == true)
                    nbColView = 0;

                ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
            }
        }

        #region Columns manager

        public void ColumnAdd(JFCGridColumn Column)
        {
            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                int lastPosition = ListColHeaderRow.Count();
                ColumnAdd(Column, lastPosition);
            }
            else if (Column.Frozen == JFCGridColumn.FrozenType.End)
            {
                int lastPosition = ListColHeaderRowEnd.Count();
                ColumnAdd(Column, lastPosition);
            }
            else
            {
                int lastPosition = ListColBody.Count();
                ColumnAdd(Column, lastPosition);
            }
        }

        public void ColumnAdd(JFCGridColumn Column, int Index)
        {
            //Column.PropertyChanged += new PropertyChangedExtendedEventHandler<Object>(Column_PropertyChanged);
            Column.PropertyChanged += new System.ComponentModel.PropertyChangedEventHandler(Column_PropertyChanged);

            Grid gridHeaderTmp;

            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                gridHeaderTmp = Grid00;
                ListColHeaderRow.Insert(Index, Column);

                ColumnAddGrid(Column, Index, Grid10, TabLineH, ListColHeaderRow);

                // on ajoute la colonne dans l'entete
                ColumnDefinition ColDefH = new ColumnDefinition();
                gridHeaderTmp.ColumnDefinitions.Insert(Index, ColDefH);

                // binding de la largeur de la colonne
                Binding BindingWidthH = new Binding("ActualWidth");
                BindingWidthH.Source = Column;
                BindingWidthH.Mode = BindingMode.TwoWay;

                ColDefH.SetBinding(ColumnDefinition.WidthProperty, BindingWidthH);

                if (Column.Width.IsAuto == true)
                {
                    if (Column.ActualWidth.IsAuto)
                        Column.ActualWidth = CalculTailleMaxi(Column);
                }
            }
            else if (Column.Frozen == JFCGridColumn.FrozenType.End)
            {
                gridHeaderTmp = Grid02;
                ListColHeaderRowEnd.Insert(Index, Column);

                ColumnAddGrid(Column, Index, Grid12, TabLineF, ListColHeaderRowEnd);

                // on ajoute la colonne dans l'entete
                ColumnDefinition ColDefH = new ColumnDefinition();
                gridHeaderTmp.ColumnDefinitions.Insert(Index, ColDefH);

                // binding de la largeur de la colonne
                Binding BindingWidthH = new Binding("ActualWidth");
                BindingWidthH.Source = Column;
                BindingWidthH.Mode = BindingMode.TwoWay;
                ColDefH.SetBinding(ColumnDefinition.WidthProperty, BindingWidthH);

                if (Column.Width.IsAuto == true)
                {
                    if (Column.ActualWidth.IsAuto)
                        Column.ActualWidth = CalculTailleMaxi(Column);
                }
            }
            else
            {
                //gridHeaderTmp = Grid01;
                ListColBody.Insert(Index, Column);

                //ColumnAddGrid(Column, Index, Grid11, TabLineB, ListColBody);
                //ColumnAddGridFooter(Column, Index, Grid21, TabLineBF, ListColBody);
            }
        }

        public void RefreshSizeColumnStar(JFCGridColumn Column)
        {
            if (Column.Width.IsStar)
            {
                //GridLength wCal = CalculTailleMaxi(Column);

                //if (wCal.Value < Parent.ScrollViewerBody.ActualWidth)
                //{
                Column.ActualWidth = new GridLength(Parent.ScrollViewerBody.ActualWidth);
                //}
                //else
                //{
                //    Column.ActualWidth = wCal;
                //}
            }
        }

        public void RefreshSizeColumnStar()
        {
            foreach (var Column in ListColBody)
            {
                RefreshSizeColumnStar(Column);
            }
        }

        public void ColumnAddHeaderFooter(JFCGridColumn Column)
        {
            int lastPosition = ListColHeaderFooter.Count();
            ColumnAddHeaderFooter(Column, lastPosition);
        }

        public void ColumnAddHeaderFooter(JFCGridColumn Column, int Index)
        {
            //Column.PropertyChanged += new PropertyChangedExtendedEventHandler<Object>(Column_PropertyChanged);
            Column.PropertyChanged += new System.ComponentModel.PropertyChangedEventHandler(Column_PropertyChanged);

            Grid gridHeaderTmp;

            gridHeaderTmp = Grid20;

            ListColHeaderFooter.Insert(Index, Column);

            ColumnAddGridFooter(Column, Index, Grid20, TabLineHF, ListColHeaderFooter);

            // on crée les headers
            int level = 0;
            int levelmax = 0;
            int MaxLevelColumnTmp = NbLevelColumn(Parent.ColunmDefinitionFooter, ref level, ref levelmax);

            MaxLevelColumn = MaxLevelColumnTmp;

            gridHeaderTmp.RowDefinitions.Clear();

            for (int i = 0; i < MaxLevelColumn; i++)
            {
                gridHeaderTmp.RowDefinitions.Add(new RowDefinition());
            }

            level = 0;
            levelmax = 0;

            if (Column.Width.IsAuto == true)
            {
                if (Column.ActualWidth.IsAuto)
                    Column.ActualWidth = CalculTailleMaxi(Column);
            }
        }

        private List<JFCExpendItem> FindChildren(JFCExpendItem item)
        {
            List<JFCExpendItem> lst = new List<JFCExpendItem>();

            foreach (var it in item.Children)
            {
                lst.Add(it);
                lst.AddRange(FindChildren(it));
            }

            return lst;
        }

        private void SetupEvents(UIElement MyElement)
        {
            MyElement.MouseEnter -= MyElement_MouseEnter;
            MyElement.MouseEnter += new MouseEventHandler(MyElement_MouseEnter);

            MyElement.MouseLeave -= MyElement_MouseLeave;
            MyElement.MouseLeave += new MouseEventHandler(MyElement_MouseLeave);

            MyElement.MouseLeftButtonDown -= MyElement_MouseLeftButtonDown;
            MyElement.MouseLeftButtonDown += new MouseButtonEventHandler(MyElement_MouseLeftButtonDown);

            MyElement.MouseLeftButtonUp -= MyElement_MouseLeftButtonUp;
            MyElement.MouseLeftButtonUp += new MouseButtonEventHandler(MyElement_MouseLeftButtonUp);

            MyElement.MouseRightButtonUp -= MyElement_MouseRightButtonUp;
            MyElement.MouseRightButtonUp += new MouseButtonEventHandler(MyElement_MouseRightButtonUp);

            MyElement.MouseDown -= MyElement_MouseDown;
            MyElement.MouseDown += new MouseButtonEventHandler(MyElement_MouseDown);

        }

        private void DeleteEvents(UIElement MyElement, Boolean IsContentGrouping = false)
        {
            MyElement.MouseEnter -= MyElement_MouseEnter;

            MyElement.MouseLeave -= MyElement_MouseLeave;

            MyElement.MouseLeftButtonDown -= MyElement_MouseLeftButtonDown;

            MyElement.MouseLeftButtonUp -= MyElement_MouseLeftButtonUp;

            MyElement.MouseRightButtonUp -= MyElement_MouseRightButtonUp;

            MyElement.MouseDown -= MyElement_MouseDown;

            if (MyElement is CellImg && IsContentGrouping == false)
            {
                ((CellImg)MyElement).ResetEvent();
            }
        }

        void MyElement_MouseEnter(object sender, MouseEventArgs e)
        {
            FrameworkElement fe = sender as FrameworkElement;

            if (fe is JFCGridItem)
            {
                ((JFCGridItem)fe).MouseEnterItem();
            }
            else
            {
                List<LinkedList<UIElement>> TabLine = null;
                IEnumerable<JFCGridColumn> TabColumn = null;

                if (fe.Parent == Grid10)
                {
                    TabLine = TabLineH;
                    TabColumn = ListColHeaderRow;
                }
                else if (fe.Parent == Grid11)
                {
                    TabLine = TabLineB;
                    TabColumn = ListColBody;
                }
                else if (fe.Parent == Grid12)
                {
                    TabLine = TabLineF;
                    TabColumn = ListColHeaderRowEnd;
                }
                else if (fe.Parent == Grid20)
                {
                    TabLine = TabLineHF;
                    TabColumn = ListColHeaderFooter;
                }
                else if (fe.Parent == Grid21)
                {
                    TabLine = TabLineBF;
                    TabColumn = ListColBody;
                }

                if (TabLine != null)
                {
                    bool flag = true;

                    if (fe is Line)
                    {
                        Line border = fe as Line;

                        if (border.Orientation == Orientation.Vertical)
                            flag = false;
                    }

                    if (flag)
                    {
                        int idxr = (int)fe.GetValue(Grid.RowProperty);

                        var lst = from ctrl in TabLine[idxr]
                                  where ctrl is JFCGridItem
                                  select ctrl;

                        if (lst.Count() > 0)
                        {
                            foreach (JFCGridItem item in lst)
                            {
                                if (item.Orientation == Orientation.Horizontal)
                                    item.MouseEnterItem();
                            }
                        }
                    }
                }

                if (TabColumn != null)
                {
                    bool flag = true;

                    if (fe is Line)
                    {
                        Line border = fe as Line;

                        if (border.Orientation == Orientation.Horizontal)
                            flag = false;
                    }

                    if (flag)
                    {
                        int idxc = (int)fe.GetValue(Grid.ColumnProperty);

                        var col = TabColumn.ElementAt(idxc);

                        if (col.Item != null)
                        {
                            col.Item.MouseEnterItem();
                        }
                    }
                }
            }
        }

        void MyElement_MouseLeave(object sender, MouseEventArgs e)
        {
            FrameworkElement fe = sender as FrameworkElement;

            if (fe is JFCGridItem)
            {
                ((JFCGridItem)fe).MouseLeaveItem();
            }
            else
            {
                List<LinkedList<UIElement>> TabLine = null;
                IEnumerable<JFCGridColumn> TabColumn = null;

                if (fe.Parent == Grid10)
                {
                    TabLine = TabLineH;
                    TabColumn = ListColHeaderRow;
                }
                else if (fe.Parent == Grid11)
                {
                    TabLine = TabLineB;
                    TabColumn = ListColBody;
                }
                else if (fe.Parent == Grid12)
                {
                    TabLine = TabLineF;
                    TabColumn = ListColHeaderRowEnd;
                }
                else if (fe.Parent == Grid20)
                {
                    TabLine = TabLineHF;
                    TabColumn = ListColHeaderFooter;
                }
                else if (fe.Parent == Grid21)
                {
                    TabLine = TabLineBF;
                    TabColumn = ListColBody;
                }

                if (fe is Line)
                {
                    Line border = sender as Line;

                    if (border.Orientation == Orientation.Horizontal)
                        TabColumn = null;
                    else
                        TabLine = null;
                }

                if (TabLine != null)
                {
                    int idxr = (int)fe.GetValue(Grid.RowProperty);

                    var lst = from ctrl in TabLine[idxr]
                              where ctrl is JFCGridItem
                              select ctrl;

                    if (lst.Count() > 0)
                    {
                        foreach (JFCGridItem item in lst)
                        {
                            if (item.Orientation == Orientation.Horizontal)
                                item.MouseLeaveItem();
                        }
                    }
                }


                if (TabColumn != null)
                {
                    int idxc = (int)fe.GetValue(Grid.ColumnProperty);

                    var col = TabColumn.ElementAt(idxc);

                    if (col.Item != null)
                    {
                        col.Item.MouseLeaveItem();
                    }
                }
            }
        }

        void MyElement_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement fe = sender as FrameworkElement;

            if (fe.Tag is HierarchicalItem || fe.Tag is JFCGridCellGrouping)
                fe = (FrameworkElement)fe.Tag;

            int idxr = (int)fe.GetValue(Grid.RowProperty);

            List<LinkedList<UIElement>> TabLine = null;

            if (fe.Parent == Grid10)
                TabLine = TabLineH;
            else if (fe.Parent == Grid11)
                TabLine = TabLineB;
            else if (fe.Parent == Grid12)
                TabLine = TabLineF;
            else if (fe.Parent == Grid20)
                TabLine = TabLineHF;
            else if (fe.Parent == Grid21)
                TabLine = TabLineBF;

            if (TabLine != null)
            {
                if (fe is JFCGridCellGrouping)
                {
                    JFCGridCellGrouping grp = fe as JFCGridCellGrouping;

                    var lstChildren = FindChildren((JFCExpendItem)grp.DataContext);

                    if (Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl))
                    {
                        if (Parent.SelectedExpendItems.Contains(((JFCExpendItem)grp.DataContext)))
                        {
                            Parent.SelectedExpendItems.Remove(((JFCExpendItem)grp.DataContext));

                            foreach (var it in lstChildren)
                            {
                                Parent.SelectedExpendItemsChildren.Remove(it);
                            }

                            // déclenche l'évennement
                            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());
                        }
                        else
                        {
                            if (Parent.SelectedExpendItems.Count > 0 && Parent.SelectedExpendItems.ElementAt(0).Level == ((JFCExpendItem)grp.DataContext).Level)
                            {
                                // on ajoute Item à la liste                        
                                Parent.SelectedExpendItems.Add(((JFCExpendItem)grp.DataContext));

                                foreach (var it in lstChildren)
                                {
                                    Parent.SelectedExpendItemsChildren.Add(it);
                                }

                                // déclenche l'évennement
                                Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());
                            }
                        }
                    }
                    else
                    {

                        // on supprime toute la selection            
                        Parent.SelectedExpendItems.RemoveWhere(row => true);

                        Parent.SelectedExpendItemsChildren.Clear();

                        // on ajoute Item à la liste                        
                        Parent.SelectedExpendItems.Add(((JFCExpendItem)grp.DataContext));

                        foreach (var it in lstChildren)
                        {
                            Parent.SelectedExpendItemsChildren.Add(it);
                        }

                        // déclenche l'évennement
                        Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());
                        //}
                    }

                    RefreshSelection();

                }
                else
                {
                    Parent.SelectedExpendItemsChildren.Clear();

                    var lst = from ctrl in TabLine[idxr]
                              where ctrl is JFCGridItem
                              select ctrl;

                    if (lst.Count() > 0)
                        item_MouseLeftButtonDown(((JFCGridItem)lst.First()), e);
                }
            }
        }

        void MyElement_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement fe = sender as FrameworkElement;

            if (fe.Tag is HierarchicalItem || fe.Tag is JFCGridCellGrouping)
                fe = (FrameworkElement)fe.Tag;

            int idxr = (int)fe.GetValue(Grid.RowProperty);

            List<LinkedList<UIElement>> TabLine = null;

            if (fe.Parent == Grid10)
                TabLine = TabLineH;
            else if (fe.Parent == Grid11)
                TabLine = TabLineB;
            else if (fe.Parent == Grid12)
                TabLine = TabLineF;
            else if (fe.Parent == Grid20)
                TabLine = TabLineHF;
            else if (fe.Parent == Grid21)
                TabLine = TabLineBF;

            if (TabLine != null)
            {

                if (fe is JFCGridCellGrouping)
                {
                    //    JFCGridCellGrouping grp = fe as JFCGridCellGrouping;

                    //    //int start = ((JFCGridItem)lst.First()).Data.StartChildIndex;
                    //    //int nbchildren = ((JFCGridItem)lst.First()).Data.NbChildren;

                    //    int start = ((JFCExpendItem)grp.DataContext).StartChildViewIndex;
                    //    int nbchildren = ((JFCExpendItem)grp.DataContext).NbChildrenView;

                    //    List<JFCExpendItem> lstItem = new List<JFCExpendItem>();

                    //    for (int i = start; i < start + nbchildren; i++)
                    //    {
                    //        //var lst1 = from ctrl in TabLine[i]
                    //        //          where ctrl is JFCGridItem
                    //        //          select ctrl;
                    //        //var it = lst1.First();
                    //        //lstItem.Add(((JFCGridItem)it).Data);

                    //        lstItem.Add(Parent.dataSource[i]);
                    //    }


                    //    if (Parent.SelectionMode == SelectionMode.Extended)
                    //    {
                    //        if (Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl))
                    //        {
                    //            SelectionExtendedControl(lstItem);
                    //        }
                    //        else if (!(Keyboard.IsKeyDown(Key.RightShift) || Keyboard.IsKeyDown(Key.LeftShift)))
                    //        {
                    //            SelectionSimple(lstItem);
                    //        }

                    //        RefreshSelection();
                    //    }

                }
                else
                {
                    var lst = from ctrl in TabLine[idxr]
                              where ctrl is JFCGridItem
                              select ctrl;

                    if (lst.Count() > 0)
                    {
                        item_MouseLeftButtonUp(((JFCGridItem)lst.First()), e);

                        e.Handled = true;
                    }
                }
            }
        }

        void MyElement_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement fe = sender as FrameworkElement;

            int idxr = (int)fe.GetValue(Grid.RowProperty);

            List<LinkedList<UIElement>> TabLine = null;

            if (fe.Parent == Grid10)
                TabLine = TabLineH;
            else if (fe.Parent == Grid11)
                TabLine = TabLineB;
            else if (fe.Parent == Grid12)
                TabLine = TabLineF;
            else if (fe.Parent == Grid20)
                TabLine = TabLineHF;
            else if (fe.Parent == Grid21)
                TabLine = TabLineBF;

            if (TabLine != null)
            {
                if (fe is JFCGridCellGrouping)
                { }
                else
                {
                    var lst = from ctrl in TabLine[idxr]
                              where ctrl is JFCGridItem
                              select ctrl;

                    if (lst.Count() > 0)
                        item_MouseRightButtonUp(((JFCGridItem)lst.First()), e);
                }
            }
        }

        void MyElement_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2)
            {
                FrameworkElement fe = sender as FrameworkElement;

                JFCGridItem item = null;

                if (fe is JFCGridItem)
                {
                    item = ((JFCGridItem)fe);
                }
                else
                {
                    int idxr = (int)fe.GetValue(Grid.RowProperty);

                    List<LinkedList<UIElement>> TabLine = null;

                    if (fe.Parent == Grid10)
                        TabLine = TabLineH;
                    else if (fe.Parent == Grid11)
                        TabLine = TabLineB;
                    else if (fe.Parent == Grid12)
                        TabLine = TabLineF;
                    else if (fe.Parent == Grid20)
                        TabLine = TabLineHF;
                    else if (fe.Parent == Grid21)
                        TabLine = TabLineBF;

                    if (TabLine != null)
                    {
                        var lst = from ctrl in TabLine[idxr]
                                  where ctrl is JFCGridItem
                                  select ctrl;

                        if (lst.Count() > 0)
                            item = ((JFCGridItem)lst.First());
                    }
                }

                if (item != null)
                {
                    JFCExpendItem it = item.Data as JFCExpendItem;

                    if (it != null)
                    {
                        if (it.IsExpendable)
                            if (it.IsExpended)
                                it.IsExpended = false;
                            else
                                it.IsExpended = true;
                    }
                }
            }
        }

        private void ColumnAddGrid(JFCGridColumn Column, int Index, Grid BodyGrid, List<LinkedList<UIElement>> LstCells, IList<JFCGridColumn> LstCol)
        {
            int NbRow = BodyGrid.RowDefinitions.Count();

            // on récupère les UIElement de la colonne à déplacer
            IEnumerable<UIElement> ListCtrl = BodyGrid.Children.Cast<UIElement>();

            var ctrls = from ctrl in ListCtrl
                        where (int)ctrl.GetValue(Grid.ColumnProperty) >= Index
                        select ctrl;

            List<UIElement> lctrls = ctrls.ToList();

            // on déplace les autres colonnes
            int idxCol;
            foreach (var ctrl in lctrls)
            {
                if (ctrl is Line)
                {
                    Line border = ctrl as Line;
                    if (border.Orientation == Orientation.Vertical)
                    {
                        idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);
                        idxCol++;
                        ctrl.SetValue(Grid.ColumnProperty, idxCol);
                    }
                }
                else if (ctrl is JFCGridItem)
                {
                    JFCGridItem item = ctrl as JFCGridItem;
                    if (item.Orientation == Orientation.Vertical)
                    {
                        idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);
                        idxCol++;
                        ctrl.SetValue(Grid.ColumnProperty, idxCol);
                    }
                }
                else// if (!(ctrl is JFCGridItem))
                {
                    idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);
                    idxCol++;
                    ctrl.SetValue(Grid.ColumnProperty, idxCol);
                }
            }

            //JFCGridItem[] TabItem = new JFCGridItem[NbRow];

            //var ctrlsItems = from ctrl in ListCtrl
            //                 where ctrl is JFCGridItem && ((JFCGridItem)ctrl).Orientation == Orientation.Horizontal
            //                 select ctrl;

            //// on récupère les JFCGridItem
            //foreach (var ctrl in ctrlsItems)
            //{
            //    if (ctrl is JFCGridItem)
            //    {
            //        if (((JFCGridItem)ctrl).Orientation == Orientation.Horizontal)
            //        {
            //            int row = (int)ctrl.GetValue(Grid.RowProperty);
            //            TabItem[row] = (JFCGridItem)ctrl;
            //        }
            //    }
            //}

            // on ajoute la colonne
            ColumnDefinition ColDef = new ColumnDefinition();
            BodyGrid.ColumnDefinitions.Insert(Index, ColDef);

            Binding BindingWidth = new Binding("ActualWidth");
            BindingWidth.Source = Column;
            BindingWidth.Mode = BindingMode.TwoWay;
            ColDef.SetBinding(ColumnDefinition.WidthProperty, BindingWidth);

            // on ajoute une ligne de séparation des colonnes
            Line bd = new Line();

            bd.LineBrush = Parent.VerticalBorderColor;

            bd.Thickness = 1;
            bd.Orientation = Orientation.Vertical;
            bd.HorizontalAlignment = HorizontalAlignment.Right;

            bd.SetValue(Grid.ColumnProperty, Index);
            if (NbRow > 0)
                bd.SetValue(Grid.RowSpanProperty, NbRow);
            else
                bd.SetValue(Grid.RowSpanProperty, 1);

            // on ajoute la ligne
            BodyGrid.Children.Add(bd);

            // ajout de l'item colonne
            Column.Item.SetValue(Grid.ColumnProperty, Index);
            if (NbRow > 0)
                Column.Item.SetValue(Grid.RowSpanProperty, NbRow);
            else
                Column.Item.SetValue(Grid.RowSpanProperty, 1);

            Column.Item.SetValue(Grid.ZIndexProperty, -10);

            BodyGrid.Children.Add(Column.Item);


            //
            JFCGridItem[] TabItem = new JFCGridItem[NbRow];

            var ctrlsItems = from ctrl in ListCtrl
                             where ctrl is JFCGridItem && ((JFCGridItem)ctrl).Orientation == Orientation.Horizontal
                             select ctrl;

            foreach (var ctrl in ctrlsItems)
            {
                int row = (int)ctrl.GetValue(Grid.RowProperty);
                TabItem[row] = (JFCGridItem)ctrl;

                ctrl.SetValue(Grid.ColumnSpanProperty, BodyGrid.ColumnDefinitions.Count());
            }

            int NbCol = BodyGrid.ColumnDefinitions.Count();

            // on boucle sur toutes les lignes
            for (int idxRow = 0; idxRow < NbRow; idxRow++)
            {
                int IndexData = Parent.ScrollVerticalValue + idxRow;

                if (Parent.dataSource == null)
                    break;

                if (IndexData < Parent.dataSource.Count())
                {
                    Object DataContext = Parent.dataSource[IndexData].Obj;

                    JFCGridItem item = TabItem[idxRow];

                    JFCExpendItem exItem = Parent.dataSource[IndexData];

                    SetupCellData(BodyGrid, LstCells[idxRow], Column, item, Index, idxRow, exItem);

                    RefreshData(idxRow, IndexData, Column, LstCells, exItem, LstCol);

                    SetupEvents(item);

                    // on recherche la ligne de séparation des rows
                    var ctrlsRow = from ctrl in LstCells[idxRow]
                                   where ctrl is Line && ((Line)ctrl).Orientation == Orientation.Horizontal
                                   select ctrl;

                    //List<UIElement> lctrlsRow = ctrlsRow.ToList();
                    //int NbCol = BodyGrid.ColumnDefinitions.Count();

                    foreach (var ctrl in ctrlsRow)
                    {
                        Line border = ctrl as Line;
                        //if (border.Orientation == Orientation.Horizontal)
                        {
                            //int NbCol = BodyGrid.ColumnDefinitions.Count();

                            if (NbCol > 0)
                                ctrl.SetValue(Grid.ColumnSpanProperty, NbCol);
                            else
                                ctrl.SetValue(Grid.ColumnSpanProperty, 1);

                            SetupEvents(ctrl);
                        }
                    }


                    // on agrandit le JFCGridItem

                    //var ctrlsItem = from ctrl in BodyGrid.Children.Cast<UIElement>()
                    //                where ctrl is JFCGridItem && ((JFCGridItem)ctrl).Orientation == Orientation.Horizontal
                    //                select ctrl;

                    //foreach (var it in ctrlsItem)
                    //{
                    //    //if (it.Orientation == Orientation.Horizontal)
                    //    it.SetValue(Grid.ColumnSpanProperty, BodyGrid.ColumnDefinitions.Count());
                    //}
                }
            }

            //if (lstCell.Count > 0)
            //    BodyGrid.Children.CopyTo(lstCell.ToArray(), BodyGrid.Children.Count);

        }

        private void ColumnAddGridFooter(JFCGridColumn Column, int Index, Grid BodyGrid, List<LinkedList<UIElement>> LstCells, IList<JFCGridColumn> LstCol)
        {
            int NbRow = BodyGrid.RowDefinitions.Count();

            // on récupère les UIElement de la colonne à déplacer
            IEnumerable<UIElement> ListCtrl = BodyGrid.Children.Cast<UIElement>();

            var ctrls = from ctrl in ListCtrl
                        where (int)ctrl.GetValue(Grid.ColumnProperty) >= Index
                        select ctrl;

            List<UIElement> lctrls = ctrls.ToList();

            // on déplace les autres colonnes
            int idxCol;
            foreach (var ctrl in lctrls)
            {
                if (ctrl is Line)
                {
                    Line border = ctrl as Line;
                    if (border.Orientation != Orientation.Horizontal)
                    {
                        idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);
                        idxCol++;
                        ctrl.SetValue(Grid.ColumnProperty, idxCol);
                    }
                }
                else if (!(ctrl is JFCGridItem))
                {
                    idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);
                    idxCol++;
                    ctrl.SetValue(Grid.ColumnProperty, idxCol);
                }
            }

            JFCGridItem[] TabItem = new JFCGridItem[NbRow];

            var ctrlsItems = from ctrl in ListCtrl
                             where ctrl is JFCGridItem
                             select ctrl;

            // on récupère les JFCGridItem
            foreach (var ctrl in ctrlsItems)
            {
                //if (ctrl is JFCGridItem)
                //{
                int row = (int)ctrl.GetValue(Grid.RowProperty);
                TabItem[row] = (JFCGridItem)ctrl;
                //}
            }

            // on ajoute la colonne
            ColumnDefinition ColDef = new ColumnDefinition();
            BodyGrid.ColumnDefinitions.Insert(Index, ColDef);

            Binding BindingWidth = new Binding("ActualWidth");
            BindingWidth.Source = Column;
            BindingWidth.Mode = BindingMode.TwoWay;
            ColDef.SetBinding(ColumnDefinition.WidthProperty, BindingWidth);

            // on ajoute une ligne de séparation des colonnes
            Line bd = new Line();

            bd.LineBrush = Parent.VerticalBorderColor;

            bd.Thickness = 1;
            bd.Orientation = Orientation.Vertical;
            bd.HorizontalAlignment = HorizontalAlignment.Right;

            bd.SetValue(Grid.ColumnProperty, Index);
            if (NbRow > 0)
                bd.SetValue(Grid.RowSpanProperty, NbRow);
            else
                bd.SetValue(Grid.RowSpanProperty, 1);

            // on ajoute la ligne
            BodyGrid.Children.Add(bd);

            // on boucle sur toutes les lignes
            for (int idxRow = 0; idxRow < NbRow; idxRow++)
            {

                if (Parent.dataSource == null)
                    break;

                if (idxRow < Parent.dataSourceFooter.Count())
                {
                    Object DataContext = Parent.dataSourceFooter[idxRow].Obj;

                    JFCGridItem item = TabItem[idxRow];

                    JFCExpendItem exItem = Parent.dataSourceFooter[idxRow];

                    SetupCellData(BodyGrid, LstCells[idxRow], Column, item, Index, idxRow, exItem);

                    RefreshData(idxRow, idxRow, Column, LstCells, exItem, LstCol);

                    // on enlève pour JFCGridItem
                    //SetupEvents(item);

                    // on recherche la ligne de séparation des rows
                    var ctrlsRow = from ctrl in LstCells[idxRow]
                                   where ctrl is Line && ((Line)ctrl).Orientation == Orientation.Horizontal
                                   select ctrl;

                    //List<UIElement> lctrlsRow = ctrlsRow.ToList();

                    foreach (var ctrl in ctrlsRow)
                    {
                        Line border = ctrl as Line;
                        //if (border.Orientation == Orientation.Horizontal)
                        {
                            int NbCol = BodyGrid.ColumnDefinitions.Count();

                            if (NbCol > 0)
                                ctrl.SetValue(Grid.ColumnSpanProperty, NbCol);
                            else
                                ctrl.SetValue(Grid.ColumnSpanProperty, 1);

                            SetupEvents(ctrl);
                        }
                    }


                    // on agrandit le JFCGridItem

                    IEnumerable<UIElement> ListCtrlItem = BodyGrid.Children.Cast<UIElement>();
                    var ctrlsItem = from ctrl in ListCtrlItem
                                    where ctrl is JFCGridItem
                                    select ctrl;

                    //List<UIElement> ListItem = ctrlsItem.ToList();

                    foreach (var it in ctrlsItem.Cast<JFCGridItem>())
                    {
                        if (it.Orientation == Orientation.Horizontal)
                            it.SetValue(Grid.ColumnSpanProperty, BodyGrid.ColumnDefinitions.Count());
                    }
                }
            }

            //if (lstCell.Count > 0)
            //    BodyGrid.Children.CopyTo(lstCell.ToArray(), BodyGrid.Children.Count);
        }

        void Column_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Width")
            {
                JFCGridColumn Col = sender as JFCGridColumn;

                if (Col.Width.IsAuto)
                    Col.ActualWidth = CalculTailleMaxi(Col);
            }
            //else if (e.PropertyName == "ActualWidth")
            //{
            //    JFCGridColumn Col = sender as JFCGridColumn;

            //    if (!Col.Width.IsAuto)
            //    {
            //        NbColView = Parent.NbColumnAffichable();
            //    }
            //}
            else if (e.PropertyName == "TypeColumn")
            {
                JFCGridColumn Col = sender as JFCGridColumn;

                if (Col.TypeColumn == JFCGridColumn.TypesColumn.Hierarchical)
                {
                    Grid gridtmp;
                    List<LinkedList<UIElement>> tabtmp;
                    IList<JFCGridColumn> LstCol;

                    int idxCol = ColumnIndexOf(Col);

                    if (idxCol == -1)
                        return;

                    if (Col.Frozen == JFCGridColumn.FrozenType.Start)
                    {
                        gridtmp = Grid10;
                        tabtmp = TabLineH;
                        LstCol = ListColHeaderRow;
                    }
                    else if (Col.Frozen == JFCGridColumn.FrozenType.End)
                    {
                        gridtmp = Grid12;
                        tabtmp = TabLineF;
                        LstCol = ListColHeaderRowEnd;
                    }
                    else
                    {
                        gridtmp = Grid11;
                        tabtmp = TabLineB;
                        LstCol = ListColBody;
                    }

                    int NbRow = gridtmp.RowDefinitions.Count();

                    if (NbRow == 0)
                        return;

                    JFCGridItem[] TabItem = new JFCGridItem[NbRow];

                    IEnumerable<UIElement> ListCtrl = gridtmp.Children.Cast<UIElement>();

                    var ctrlsItems = from ctrl in ListCtrl
                                     where ctrl is JFCGridItem
                                     select ctrl;

                    // on récupère les JFCGridItem
                    foreach (var ctrl in ctrlsItems)
                    {
                        int row = (int)ctrl.GetValue(Grid.RowProperty);
                        TabItem[row] = (JFCGridItem)ctrl;
                    }

                    int idx = 0;
                    foreach (var lst in tabtmp)
                    {

                        List<UIElement> lstremove = new List<UIElement>();

                        var l = from ctrl in lst
                                where (int)ctrl.GetValue(Grid.ColumnProperty) == idxCol
                                select ctrl;

                        foreach (var item in l)
                        {
                            if (item is CellImg || item is HierarchicalItem || item is JFCGridCell)
                            {
                                lstremove.Add(item);
                            }
                        }


                        foreach (var itremove in lstremove)
                        {
                            DeleteEvents(itremove);

                            tabtmp[idx].Remove(itremove);

                            gridtmp.Children.Remove(itremove);
                        }

                        idx++;
                    }

                    // on boucle sur toutes les lignes
                    for (int idxRow = 0; idxRow < NbRow; idxRow++)
                    {
                        int IndexData = Parent.ScrollVerticalValue + idxRow;
                        Object DataContext = Parent.dataSource[IndexData].Obj;

                        JFCGridItem item = TabItem[idxRow];

                        JFCExpendItem exItem = Parent.dataSource[IndexData];

                        SetupCellData(gridtmp, tabtmp[idxRow], Col, item, idxCol, idxRow, exItem);

                        RefreshData(idxRow, IndexData, Col, tabtmp, exItem, LstCol);

                        // on enlève pour JFCGridItem
                        //SetupEvents(item);
                    }
                }
            }
        }

        private void ColumnAddGridHeader(Grid grid, JFCGridColumn Column, int Index)
        {
            // on ajoute la colonne dans l'entete
            ColumnDefinition ColDefH = new ColumnDefinition();
            grid.ColumnDefinitions.Insert(Index, ColDefH);

            // binding de la largeur de la colonne
            Binding BindingWidthH = new Binding("ActualWidth");
            BindingWidthH.Source = Column;
            BindingWidthH.Mode = BindingMode.TwoWay;
            ColDefH.SetBinding(ColumnDefinition.WidthProperty, BindingWidthH);
        }

        private void ColumnRemoveGridHeader(Grid grid, int Index)
        {
            // on supprime la colonne dans l'entete
            grid.ColumnDefinitions.RemoveAt(Index);
        }

        public void RefreshHeaderColumnsWidth()
        {
            GridLength len;

            foreach (JFCGridColumn Col in ListColHeaderRow)
            {
                if (Col.Width.IsAuto)// || Col.Width.Value == -1)
                {
                    len = CalculTailleMaxi(Col);

                    if (Col.ActualWidth.Value < len.Value)
                        Col.ActualWidth = len;
                }
            }
        }

        public void RefreshAllColumnsWidth()
        {
            GridLength len;

            //RefreshHeaderColumnsWidth();

            foreach (JFCGridColumn Col in ListColHeaderRow)
            {
                if (Col.Width.IsAuto)// || Col.Width.Value == -1)
                {
                    len = CalculTailleMaxi(Col);

                    if (Col.ActualWidth.Value < len.Value)
                        Col.ActualWidth = len;
                }
            }

            if (Parent.IsVirtualised)
            {
                int index = 0;

                foreach (JFCGridColumn Col in ListColBody)
                {
                    if (startIndexColView <= index && index < startIndexColView + nbColView)
                    {
                        if (Col.Width.IsAuto)// || Col.Width.Value == -1)
                        {
                            len = CalculTailleMaxi(Col);

                            if (Col.ActualWidth.Value < len.Value)
                                Col.ActualWidth = len;
                        }
                    }

                    index++;
                }
            }
            else
            {
                foreach (JFCGridColumn Col in ListColBody)
                {
                    if (Col.Width.IsAuto)// || Col.Width.Value == -1)
                    {
                        len = CalculTailleMaxi(Col);

                        if (Col.ActualWidth.Value < len.Value)
                            Col.ActualWidth = len;
                    }
                }
            }
        }

        public void ColumnMove(JFCGridColumn Column, int Index)
        {
            IList<JFCGridColumn> ListColumns;
            Grid gridTmp;
            Grid gridHeaderTmp;

            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                if (Column.Frozen == JFCGridColumn.FrozenType.Start)
                {
                    gridTmp = Grid10;
                    gridHeaderTmp = Grid00;
                    ListColumns = ListColHeaderRow;
                }
                else if (Column.Frozen == JFCGridColumn.FrozenType.End)
                {
                    gridTmp = Grid12;
                    gridHeaderTmp = Grid02;
                    ListColumns = ListColHeaderRowEnd;
                }
                else
                {
                    gridTmp = Grid11;
                    gridHeaderTmp = Grid01;
                    ListColumns = ListColBody;
                }

                int indexBefore = ListColumns.IndexOf(Column);

                if (Index == indexBefore)
                    return;

                ListColumns.Remove(Column);

                IEnumerable<UIElement> ListCtrl = gridTmp.Children.Cast<UIElement>();
                var ctrls = from ctrl in ListCtrl
                            where (int)ctrl.GetValue(Grid.ColumnProperty) == indexBefore
                            select ctrl;

                List<UIElement> lctrls = ctrls.ToList();

                IEnumerable<UIElement> ctrlsOtherColumns;

                // on récupère les colonnes à déplacer
                if (indexBefore < Index)
                {
                    ListCtrl = gridTmp.Children.Cast<UIElement>();

                    ctrlsOtherColumns = from ctrl in ListCtrl
                                        where (int)ctrl.GetValue(Grid.ColumnProperty) > indexBefore && (int)ctrl.GetValue(Grid.ColumnProperty) <= Index
                                        select ctrl;

                }
                else
                {
                    ListCtrl = gridTmp.Children.Cast<UIElement>();

                    ctrlsOtherColumns = from ctrl in ListCtrl
                                        where (int)ctrl.GetValue(Grid.ColumnProperty) < indexBefore && (int)ctrl.GetValue(Grid.ColumnProperty) >= Index
                                        select ctrl;
                }

                List<UIElement> lctrlsOtherColumns = ctrlsOtherColumns.ToList();

                // on supprime la colonne du grid
                ColumnDefinition ColDef = gridTmp.ColumnDefinitions[indexBefore];
                gridTmp.ColumnDefinitions.RemoveAt(indexBefore);

                ColumnDefinition ColDefH = gridHeaderTmp.ColumnDefinitions[indexBefore];
                gridHeaderTmp.ColumnDefinitions.RemoveAt(indexBefore);

                int idxCol;
                Boolean move = false;

                // on déplace les autres colonnes
                foreach (var ctrl in lctrlsOtherColumns)
                {
                    move = true;

                    if (ctrl is JFCGridItem)
                    {
                        if (((JFCGridItem)ctrl).Orientation == Orientation.Horizontal)
                            move = false;
                        else
                            move = true;
                    }
                    else if (ctrl is Line)
                    {
                        Line b = ctrl as Line;

                        if (b.Orientation == Orientation.Horizontal)
                            move = false;
                    }

                    if (move == true)
                    {
                        if (indexBefore < Index)
                        {
                            //cell.Column--;
                            idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);
                            idxCol--;
                            ctrl.SetValue(Grid.ColumnProperty, idxCol);
                        }
                        else
                        {
                            //cell.Column++;
                            idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);
                            idxCol++;
                            ctrl.SetValue(Grid.ColumnProperty, idxCol);
                        }
                    }
                }

                // on ajoute la colonne
                gridTmp.ColumnDefinitions.Insert(Index, ColDef);
                ListColumns.Insert(Index, Column);

                gridHeaderTmp.ColumnDefinitions.Insert(Index, ColDefH);

                // on déplace les cellule de la colonne la colonne
                foreach (var ctrl in lctrls)
                {
                    move = true;

                    if (ctrl is JFCGridItem)
                    {
                        move = false;
                    }
                    else if (ctrl is Line)
                    {
                        Line b = ctrl as Line;

                        if (b.Orientation == Orientation.Horizontal)
                            move = false;
                    }

                    if (move == true)
                    {
                        ctrl.SetValue(Grid.ColumnProperty, Index);
                    }
                }

                // on crée les headers
                int level = 0;
                int levelmax = 0;
                int MaxLevelColumnTmp = NbLevelColumn(Parent.ColunmDefinition, ref level, ref levelmax);

                MaxLevelColumn = MaxLevelColumnTmp;

                gridHeaderTmp.RowDefinitions.Clear();

                for (int i = 0; i < MaxLevelColumn; i++)
                {
                    gridHeaderTmp.RowDefinitions.Add(new RowDefinition());
                }

                level = 0;
                levelmax = 0;
                //TODO
                Grid00.Children.Clear();
                Grid01.Children.Clear();
                levelmax = BuildColumnHeader(Parent.ColunmDefinition, ref level, ref levelmax);
            }
            else
            {
                int indexBefore = ListColBody.IndexOf(Column);
                ListColBody.Move(indexBefore, Index);
            }

        }

        public void ColumnRemove(JFCGridColumn Column)
        {
            Column.PropertyChanged -= Column_PropertyChanged;

            //List<JFCGridColumn> ListColumns;
            //Grid gridTmp;
            //Grid gridHeaderTmp;
            //List<List<UIElement>> TabLineTmp;

            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                //gridTmp = Grid10;
                //gridHeaderTmp = Grid00;
                //ListColumns = ListColHeaderRow;
                //TabLineTmp = TabLineH;

                int index = ListColHeaderRow.IndexOf(Column);

                if (index == -1)
                    return;

                // on supprime la colonne                
                Grid00.ColumnDefinitions.RemoveAt(index);
                ListColHeaderRow.Remove(Column);

                ColumnRemove(index, Grid10, TabLineH);
            }
            if (Column.Frozen == JFCGridColumn.FrozenType.End)
            {
                int index = ListColHeaderRowEnd.IndexOf(Column);

                if (index == -1)
                    return;

                // on supprime la colonne                
                Grid02.ColumnDefinitions.RemoveAt(index);
                ListColHeaderRowEnd.Remove(Column);

                ColumnRemove(index, Grid12, TabLineF);
            }
            else
            {
                //gridTmp = Grid11;
                //gridHeaderTmp = Grid01;
                //ListColumns = ListColBody;
                //TabLineTmp = TabLineB;

                int index = ListColBody.IndexOf(Column);

                if (index == -1)
                    return;

                // on supprime la colonne                
                //Grid01.ColumnDefinitions.RemoveAt(index);
                ListColBody.Remove(Column);

                ////ColumnRemove(index, ListColBody, Grid11, TabLineB);
                ////ColumnRemove(index, ListColBody, Grid21, TabLineBF);

                //ColumnRemove(index, Grid11, TabLineB);
                //ColumnRemove(index, Grid21, TabLineBF);
            }
        }

        private void ColumnRemove(int index, Grid gridTmp, List<LinkedList<UIElement>> TabLineTmp)
        {
            ColumnRemove(index, gridTmp, TabLineTmp, false);
        }

        private void ColumnRemove(int index, Grid gridTmp, List<LinkedList<UIElement>> TabLineTmp, bool useLstRemoveCol)
        {
            //useLstRemoveCol = false;

            if (index < gridTmp.ColumnDefinitions.Count())
            {
                gridTmp.ColumnDefinitions.RemoveAt(index);

                int idxRow;
                int idxCol;

                int NbCol = gridTmp.ColumnDefinitions.Count();

                // on teste si le nombre de colonnes est égale à 0
                // car le ColumnSpan ne peut pas etre à 0
                if (NbCol == 0)
                    NbCol = 1;

                IEnumerable<UIElement> ListCtrl = gridTmp.Children.Cast<UIElement>().ToList();

                foreach (var ctrl in ListCtrl)
                {
                    //if (!(ctrl is JFCGridItem))
                    //{
                    int idx = (int)ctrl.GetValue(Grid.ColumnProperty);
                    if (idx == index)
                    {
                        if (ctrl is Line)
                        {
                            Line bd = ctrl as Line;

                            if (bd.Orientation == Orientation.Vertical)
                                gridTmp.Children.Remove(bd);
                        }
                        else if (ctrl is JFCGridItem)
                        {
                            JFCGridItem item = ctrl as JFCGridItem;

                            if (item.Orientation == Orientation.Vertical)
                                gridTmp.Children.Remove(ctrl);
                            else
                                ctrl.SetValue(Grid.ColumnSpanProperty, NbCol);
                        }
                        else
                        {
                            if (useLstRemoveCol)
                            {
                                idxRow = (int)ctrl.GetValue(Grid.RowProperty);

                                bool removed = false;

                                if (idxRow < TabLineTmp.Count())
                                    removed = TabLineTmp[idxRow].Remove(ctrl);

                                DeleteEvents(ctrl);

                                if (removed)
                                {
                                    if (ctrl is CellImg)
                                        LstRemoveColCellImg.Add((CellImg)ctrl);
                                    else if (ctrl is JFCGridCell)
                                        LstRemoveColJFCGridCell.Add((JFCGridCell)ctrl);
                                }
                                else
                                {
                                    bool finded = false;

                                    if (ctrl is CellImg)
                                    {
                                        if (LstRemoveColCellImg.Contains((CellImg)ctrl))
                                            finded = true;
                                    }
                                    else if (ctrl is JFCGridCell)
                                    {
                                        if (LstRemoveColJFCGridCell.Contains((JFCGridCell)ctrl))
                                            finded = true;
                                    }

                                    if (finded == false)
                                        gridTmp.Children.Remove(ctrl);
                                }
                            }
                            else
                            {
                                gridTmp.Children.Remove(ctrl);
                                idxRow = (int)ctrl.GetValue(Grid.RowProperty);

                                if (idxRow < TabLineTmp.Count())
                                    TabLineTmp[idxRow].Remove(ctrl);

                                DeleteEvents(ctrl);

                                if (ctrl is CellImg)
                                    LstRecycleCellImg.Add((CellImg)ctrl);
                                else if (ctrl is JFCGridCell)
                                    LstRecycleJFCGridCell.Add((JFCGridCell)ctrl);
                            }
                        }
                    }
                    else if (idx >= index)
                    {
                        idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);
                        idxCol--;
                        ctrl.SetValue(Grid.ColumnProperty, idxCol);
                    }
                    //}
                    //else
                    //{
                    //    ctrl.SetValue(Grid.ColumnSpanProperty, NbCol);
                    //}
                }

                //if (Parent.IsVirtualised == true)
                //{
                //    if (startIndexColView + nbColView > NbColumn(false))
                //    {
                //        //double width = 0;

                //        //for (int i = NbCol; i >= 0; i--)
                //        //{
                //        //    width += ColumnWidth(false, i);
                //        //}

                //        Parent.RefreshScrollBarHorizontal(true);

                //    }
                //}
            }

        }

        public void ColumnRemoveAll()
        {
            foreach (var colh in ListColHeaderRow.ToList())
            {
                ColumnRemove(colh);
            }

            BuildHeader(Grid00);


            ListColBody.Clear();

            BuildHeader(Grid01);

            foreach (var colh in ListColHeaderRowEnd.ToList())
            {
                ColumnRemove(colh);
            }

            BuildHeader(Grid02);

            ColumnRemoveFooterAll();
        }

        public void ColumnRemoveAll(bool frozen)
        {
            if (frozen)
            {
                foreach (var colh in ListColHeaderRow.ToList())
                {
                    ColumnRemove(colh);
                }

                BuildHeader(Grid00);

                foreach (var colh in ListColHeaderRowEnd.ToList())
                {
                    ColumnRemove(colh);
                }

                BuildHeader(Grid02);
            }
            else
            {
                //foreach (var colb in ListColBody.ToList())
                //{
                //    ColumnRemove(colb);
                //}

                ListColBody.Clear();

                //if (Parent.IsVirtualised == true)
                //{
                //    startIndexColView = 0;
                //    nbColView = 0;
                //}

                //BuildHeader(Grid01);
            }
        }

        public void ColumnRemoveFooter(JFCGridColumn Column)
        {
            Column.PropertyChanged -= Column_PropertyChanged;

            int index = ListColHeaderFooter.IndexOf(Column);

            if (index == -1)
                return;

            // on supprime la colonne                
            //Grid00.ColumnDefinitions.RemoveAt(index);
            ListColHeaderFooter.Remove(Column);

            ColumnRemove(index, Grid20, TabLineHF);
        }

        public void ColumnRemoveFooterAll()
        {
            //BuildHeader(Grid02);

            foreach (var colh in ListColHeaderFooter.ToList())
            {
                ColumnRemoveFooter(colh);
            }

            BuildHeader(Grid02);
        }

        public double ColumnWidth(JFCGridColumn.FrozenType Frozen, int Index)
        {
            if (Frozen == JFCGridColumn.FrozenType.Start)
            {
                if (Index < ListColHeaderRow.Count())
                {
                    return ListColHeaderRow[Index].ActualWidth.Value;
                }
                else
                {
                    return -1;
                }
            }
            else if (Frozen == JFCGridColumn.FrozenType.End)
            {
                if (Index < ListColHeaderRowEnd.Count())
                {
                    return ListColHeaderRowEnd[Index].ActualWidth.Value;
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                if (Index < ListColBody.Count())
                {
                    JFCGridColumn Column = ListColBody[Index];

                    if (Column.Width.IsAuto == true)
                    {
                        if (Column.ActualWidth.IsAuto || Column.ActualWidth.Value == 0)
                            Column.ActualWidth = CalculTailleMaxi(Column);
                    }

                    return ListColBody[Index].ActualWidth.Value;
                }
                else
                {
                    return -1;
                }
            }
        }

        public int ColumnIndexOf(JFCGridColumn Column)
        {
            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                return ListColHeaderRow.IndexOf(Column);
            }
            else if (Column.Frozen == JFCGridColumn.FrozenType.End)
            {
                return ListColHeaderRowEnd.IndexOf(Column);
            }
            else
            {
                return ListColBody.IndexOf(Column);
            }
        }

        public JFCGridColumn ColumnBody(int Index)
        {
            if (Index < ListColBody.Count)
                return ListColBody[Index];
            else
                return null;

        }

        public int ColumnHeaderRowFooterIndexOf(JFCGridColumn Column)
        {
            return ListColHeaderFooter.IndexOf(Column);
        }

        public int NbColumn(JFCGridColumn.FrozenType Frozen)
        {
            if (Frozen == JFCGridColumn.FrozenType.Start)
            {
                return ListColHeaderRow.Count();
            }
            else if (Frozen == JFCGridColumn.FrozenType.End)
            {
                return ListColHeaderRowEnd.Count();
            }
            else
            {
                return ListColBody.Count();
            }
        }

        public int NbColumnHeaderRowFooter()
        {
            return ListColHeaderFooter.Count();
        }

        // calcul le nombre d'entete de colonne
        private int NbLevelColumn(ObservableCollection<JFCGridColumn> Columns, ref int Level, ref int MaxLevel)
        {
            Level += 1;

            if (Level > MaxLevel)
                MaxLevel = Level;

            // on boucle sur les colonne pour vérifier si on est en multi header
            foreach (JFCGridColumn col in Columns)
            {
                if (col.ChildrenColumns.Count() > 0)
                {


                    //if (col.GroupChild.NbRow == 1 && col.GroupChild.NbColumn == 1)
                    NbLevelColumn(col.ChildrenColumns, ref Level, ref MaxLevel);


                }
            }

            Level -= 1;

            return MaxLevel;
        }

        internal void ReBuildColumnHeader(JFCGridColumn.FrozenType ColumnFrozen)
        {
            Grid gridHeaderTmp;
            IEnumerable<JFCGridColumn> lstCol;

            if (ColumnFrozen == JFCGridColumn.FrozenType.Start)
            {
                gridHeaderTmp = Grid00;

                lstCol = from col in Parent.ColunmDefinition
                         where col.Frozen == JFCGridColumn.FrozenType.Start
                         select col;
            }
            else if (ColumnFrozen == JFCGridColumn.FrozenType.End)
            {
                gridHeaderTmp = Grid02;

                lstCol = from col in Parent.ColunmDefinition
                         where col.Frozen == JFCGridColumn.FrozenType.End
                         select col;
            }
            else
            {
                gridHeaderTmp = Grid01;

                lstCol = from col in Parent.ColunmDefinition
                         where col.Frozen == JFCGridColumn.FrozenType.None
                         select col;

                //lstCol = new List<JFCGridColumn>();
            }

            // on crée les headers
            int level = 0;
            int levelmax = 0;
            int MaxLevelColumnTmp = NbLevelColumn(Parent.ColunmDefinition, ref level, ref levelmax);

            MaxLevelColumn = MaxLevelColumnTmp;

            if (MaxLevelColumn != gridHeaderTmp.RowDefinitions.Count())
            {
                gridHeaderTmp.RowDefinitions.Clear();

                for (int i = 0; i < MaxLevelColumn; i++)
                {
                    gridHeaderTmp.RowDefinitions.Add(new RowDefinition());
                }
            }
            level = 0;
            levelmax = 0;


            //Grid00.Children.Clear();
            //Grid01.Children.Clear();
            //levelmax = BuildColumnHeader(Parent.ColunmDefinition, ref level, ref levelmax);

            //gridHeaderTmp.Children.Clear();

            LstRemoveHeader = (from item in gridHeaderTmp.Children.Cast<UIElement>()
                               where item is JFCGridColumnHeader
                               select item).ToList();

            LstRemoveBelowHeader = (from item in gridHeaderTmp.Children.Cast<UIElement>()
                                    where item is JFCGridColumnBelowHeader
                                    select item).ToList();

            LstRemoveSplitter = (from item in gridHeaderTmp.Children.Cast<UIElement>()
                                 where item is GridSplitter
                                 select item).ToList();


            levelmax = BuildColumnHeader(lstCol, ref level, ref levelmax);

            foreach (var item in LstRemoveHeader)
            {
                gridHeaderTmp.Children.Remove(item);
            }

            foreach (var item in LstRemoveBelowHeader)
            {
                gridHeaderTmp.Children.Remove(item);
            }

            foreach (var item in LstRemoveSplitter)
            {
                gridHeaderTmp.Children.Remove(item);
            }
        }

        internal void BuildHeader(Grid GridHeader)
        {
            // on crée les headers
            int level = 0;
            int levelmax = 0;
            int MaxLevelColumnTmp = NbLevelColumn(Parent.ColunmDefinition, ref level, ref levelmax);

            IEnumerable<JFCGridColumn> lcol = null;

            if (GridHeader == Grid00)
            {
                lcol = from col in Parent.ColunmDefinition
                       where col.Frozen == JFCGridColumn.FrozenType.Start
                       select col;
            }
            else if (GridHeader == Grid01)
            {
                lcol = from col in Parent.ColunmDefinition
                       where col.Frozen == JFCGridColumn.FrozenType.None
                       select col;
            }
            else if (GridHeader == Grid02)
            {
                lcol = from col in Parent.ColunmDefinition
                       where col.Frozen == JFCGridColumn.FrozenType.End
                       select col;
            }


            if (MaxLevelColumn == MaxLevelColumnTmp)
            {
                // si le nombre de niveau n'a pas changé
                level = 0;
                levelmax = 0;
                //TODO
                GridHeader.Children.Clear();
                //Grid01.Children.Clear();
                //levelmax = BuildColumnHeader(Parent.ColunmDefinition, ref level, ref levelmax);
                levelmax = BuildColumnHeader(lcol, ref level, ref levelmax);
            }
            else
            {
                // si le nombre de niveau a changé
                // on refait tout

                MaxLevelColumn = MaxLevelColumnTmp;

                GridHeader.RowDefinitions.Clear();

                for (int i = 0; i < MaxLevelColumn; i++)
                {
                    GridHeader.RowDefinitions.Add(new RowDefinition());
                }


                level = 0;
                levelmax = 0;
                //TODO
                GridHeader.Children.Clear();
                //Grid01.Children.Clear();
                //levelmax = BuildColumnHeader(Parent.ColunmDefinition, ref level, ref levelmax);
                levelmax = BuildColumnHeader(lcol, ref level, ref levelmax);
            }
        }

        private int BuildColumnHeader(IEnumerable<JFCGridColumn> Columns, ref int Level, ref int MaxLevel)
        {

            // on boucle sur les colonne pour vérifier si on est en multi header
            foreach (JFCGridColumn col in Columns)
            {
                // on met a jour la position de la colonne
                //col.Position = -1;

                if (col.ChildrenColumns.Count() > 0)
                {
                    //if (col.GroupChild.NbRow == 1 && col.GroupChild.NbColumn == 1)
                    //{
                    Level += 1;

                    if (Level > MaxLevel)
                        MaxLevel = Level;

                    BuildColumnHeader(col.ChildrenColumns, ref Level, ref MaxLevel);

                    Level -= 1;
                    //}
                }

                HeaderAdd(col, Level);

            }

            return MaxLevel;
        }

        private int CalculColSpan(JFCGridColumn col)
        {
            int Taille = 0;

            if (col.ChildrenColumns.Count() > 0)
            {
                foreach (JFCGridColumn c in col.ChildrenColumns)
                {
                    Taille += CalculColSpan(c);
                }
            }
            else
            {
                Taille = 1;
            }

            return Taille;

        }

        private void HeaderAdd(JFCGridColumn col, int Level)
        {

            //ColumnDefinition ColDef;
            int IdxCol = -1;
            Grid GridH;
            //List<JFCGridColumn> ListB;

            GridSplitter Split;

            //// on vérifie si la colonne est figée
            //if (col.Frozen == true)
            //{
            //    GridH = Grid00;

            //    ListB = ListColHeaderRow;

            //    Split = new GridSplitter();
            //}
            //else
            //{
            //    GridH = Grid01;
            //    ListB = ListColBody.ToList();

            //    Split = new GridSplitter();

            //}

            int NbCol = -1;

            if (col.Frozen == JFCGridColumn.FrozenType.Start)
            {
                GridH = Grid00;

                if (col.ChildrenColumns.Count() == 0)
                {
                    IdxCol = ListColHeaderRow.IndexOf(col);

                    NbCol = 1;
                }
                else
                {
                    NbCol = CalculColSpan(col);

                    IdxCol = ListColHeaderRow.IndexOf(FindLastChildrenColumn(col.ChildrenColumns)) - NbCol + 1;
                }

                if (IdxCol < 0)
                    IdxCol = 0;
            }
            else if (col.Frozen == JFCGridColumn.FrozenType.End)
            {
                GridH = Grid02;

                if (col.ChildrenColumns.Count() == 0)
                {
                    IdxCol = ListColHeaderRowEnd.IndexOf(col);

                    NbCol = 1;
                }
                else
                {
                    NbCol = CalculColSpan(col);

                    IdxCol = ListColHeaderRowEnd.IndexOf(FindLastChildrenColumn(col.ChildrenColumns)) - NbCol + 1;
                }

                if (IdxCol < 0)
                    IdxCol = 0;
            }
            else
            {
                GridH = Grid01;

                if (Parent.IsVirtualised == false)
                {
                    if (col.ChildrenColumns.Count() == 0)
                    {
                        IdxCol = ListColBody.IndexOf(col);

                        NbCol = 1;
                    }
                    else
                    {
                        NbCol = CalculColSpan(col);

                        IdxCol = ListColBody.IndexOf(FindLastChildrenColumn(col.ChildrenColumns)) - NbCol + 1;
                    }

                    if (IdxCol < 0)
                        IdxCol = 0;
                }
                else
                {
                    // pour la virtualisation des colonnes

                    if (NbColView == 0)
                        return;

                    if (col.ChildrenColumns.Count() == 0)
                    {
                        IdxCol = ListColBody.IndexOf(col);

                        NbCol = 1;
                    }
                    else
                    {
                        NbCol = CalculColSpan(col);

                        IdxCol = ListColBody.IndexOf(FindLastChildrenColumn(col.ChildrenColumns)) - NbCol + 1;

                        //List<JFCGridColumn> lstCol = FindChildrenColumn(col);
                        //int IdxColtmp = -1;
                        //int NbColtmp = 0;

                        //foreach (var c in lstCol)
                        //{

                        //    IdxColtmp = ListColBody.IndexOf(c);

                        //    if (IdxColtmp >= 0)
                        //    {
                        //        if (IdxCol == -1)
                        //        {
                        //            IdxCol = IdxColtmp;
                        //        }

                        //        if (IdxColtmp < IdxCol)
                        //            IdxCol = IdxColtmp;

                        //        NbColtmp++;
                        //    }
                        //}
                    }
                }

                if (NbColView == 0 && Parent.IsVirtualised == true)
                    return;

                if (IdxCol + NbCol - 1 < StartIndexColView)
                    return;

                if (IdxCol > StartIndexColView + NbColView - 1)
                    return;


                if (IdxCol < StartIndexColView)
                {
                    NbCol = NbCol - (StartIndexColView - IdxCol);

                    IdxCol = 0;
                }
                else
                {
                    IdxCol = IdxCol - StartIndexColView;
                }

                if (NbColView < IdxCol + NbCol)
                {
                    NbCol = NbColView - IdxCol;
                }

                if (IdxCol < 0)
                    return;

                if (NbCol < 0)
                    return;

            }

            //// pour la virtualisation des colonnes
            //if (col.Frozen == false)
            //{
            //    if (NbColView == 0)
            //        return;

            //    if (IdxCol + NbCol - 1 < StartIndexColView)
            //        return;

            //    if (IdxCol > StartIndexColView + NbColView - 1)
            //        return;


            //    if (IdxCol < StartIndexColView)
            //    {
            //        NbCol = NbCol - (StartIndexColView - IdxCol);

            //        IdxCol = 0;
            //    }
            //    else
            //    {
            //        IdxCol = IdxCol - StartIndexColView;
            //    }

            //    if (NbColView < IdxCol + NbCol)
            //    {
            //        NbCol = NbColView - IdxCol;
            //    }


            //}

            bool splitterRecycle = false;
            if (LstRemoveSplitter.Count() > 0)
            {
                Split = (GridSplitter)LstRemoveSplitter.Last();
                LstRemoveSplitter.RemoveAt(LstRemoveSplitter.Count() - 1);
                splitterRecycle = true;
            }
            else
            {
                Split = new GridSplitter();
            }

            // on met a jour la position de la colonne
            col.Header.SetValue(Grid.ColumnProperty, IdxCol);
            col.Header.SetValue(Grid.RowProperty, Level);

            Split.Width = 8;
            Split.IsTabStop = false;
            Split.Background = Brushes.Transparent;
            Split.SetValue(Grid.ColumnProperty, IdxCol);
            Split.SetValue(Grid.RowProperty, Level);
            Split.ShowsPreview = false;
            Split.MouseDoubleClick -= Split_MouseDoubleClick;
            Split.MouseDoubleClick += new MouseButtonEventHandler(Split_MouseDoubleClick);
            Split.DragCompleted -= Split_DragCompleted;
            Split.DragCompleted += new System.Windows.Controls.Primitives.DragCompletedEventHandler(Split_DragCompleted);
            Split.Tag = col;

            if (col.ChildrenColumns.Count() == 0)
            {
                //col.Header.SetValue(Grid.RowSpanProperty, (MaxLevelColumn - Level) + 1);
                //Split.SetValue(Grid.RowSpanProperty, (MaxLevelColumn - Level) + 1);

                col.Header.SetValue(Grid.RowSpanProperty, (MaxLevelColumn - Level));
                Split.SetValue(Grid.RowSpanProperty, (MaxLevelColumn - Level));
            }
            else
            {
                //if (NbCol == -1)
                //    NbCol = CalculColSpan(col);

                col.Header.SetValue(Grid.ColumnSpanProperty, NbCol);
                Split.SetValue(Grid.ColumnSpanProperty, NbCol);

                col.Header.SetValue(Grid.RowSpanProperty, 1);

                // on ajoute le composant graphique du fond des colonnes enfants (BelowHeader)
                //JFCGridColumnBelowHeader BelowHeader = new JFCGridColumnBelowHeader();
                col.Header.BelowHeader.SetValue(Grid.ColumnProperty, IdxCol);
                col.Header.BelowHeader.SetValue(Grid.ColumnSpanProperty, NbCol);

                col.Header.BelowHeader.SetValue(Grid.RowProperty, Level + 1);
                col.Header.BelowHeader.SetValue(Grid.RowSpanProperty, MaxLevelColumn - Level);

                //GridH.Children.Insert(0, col.Header.BelowHeader);

                if (!LstRemoveBelowHeader.Contains(col.Header.BelowHeader))
                {
                    GridH.Children.Insert(0, col.Header.BelowHeader);
                }
                else
                {
                    LstRemoveHeader.Remove(col.Header.BelowHeader);
                }

            }

            col.Header.HorizontalAlignment = HorizontalAlignment.Stretch;
            col.Header.VerticalAlignment = VerticalAlignment.Stretch;

            if (!LstRemoveHeader.Contains(col.Header))
            {
                GridH.Children.Add(col.Header);
            }
            else
            {
                LstRemoveHeader.Remove(col.Header);
            }

            if (!splitterRecycle)
                GridH.Children.Add(Split);

            if (col.IsResizable == false)
                Split.Visibility = Visibility.Hidden;
            else
                Split.Visibility = Visibility.Visible;

            col.Header.PreviewMouseLeftButtonDown -= Header_PreviewMouseLeftButtonDown;
            col.Header.PreviewMouseLeftButtonUp -= Header_PreviewMouseLeftButtonUp;
            col.Header.PreviewMouseMove -= Header_PreviewMouseMove;

            col.Header.PreviewMouseLeftButtonDown += new MouseButtonEventHandler(Header_PreviewMouseLeftButtonDown);
            col.Header.PreviewMouseLeftButtonUp += new MouseButtonEventHandler(Header_PreviewMouseLeftButtonUp);
            col.Header.PreviewMouseMove += new MouseEventHandler(Header_PreviewMouseMove);
        }

        public List<JFCGridColumn> FindChildrenColumn(ObservableCollection<JFCGridColumn> Columns)
        {

            List<JFCGridColumn> lCol = new List<JFCGridColumn>();

            // on boucle sur les colonne pour vérifier si on est en multi header
            foreach (JFCGridColumn col in Columns)
            {
                // on met a jour la position de la colonne
                //col.Position = -1;

                if (col.ChildrenColumns.Count() > 0)
                {
                    lCol.AddRange(FindChildrenColumn(col.ChildrenColumns));
                }
                else
                {
                    lCol.Add(col);
                }
            }

            return lCol;
        }

        public List<JFCGridColumn> FindChildrenColumn(JFCGridColumn Column)
        {

            List<JFCGridColumn> lCol = new List<JFCGridColumn>();

            if (Column.ChildrenColumns.Count() > 0)
            {
                lCol.AddRange(FindChildrenColumn(Column.ChildrenColumns));
            }

            return lCol;
        }

        public void BuildParentColumn(JFCGridColumn Column)
        {
            List<JFCGridColumn> lCol = new List<JFCGridColumn>();

            // on boucle sur les colonne pour vérifier si on est en multi header
            foreach (JFCGridColumn col in Column.ChildrenColumns)
            {
                // on met a jour la position de la colonne
                //col.Position = -1;

                if (col.ChildrenColumns.Count() > 0)
                {
                    FindChildrenColumn(col.ChildrenColumns);
                }
                //else
                //{
                //    col.Parent = Column;
                //}               
            }
        }

        public JFCGridColumn FindLastChildrenColumn(ObservableCollection<JFCGridColumn> Columns)
        {

            JFCGridColumn LastCol = Columns.Last();

            if (LastCol.ChildrenColumns.Count() == 0)
            {
                return LastCol;
            }
            else
            {
                return FindLastChildrenColumn(LastCol.ChildrenColumns);
            }
        }

        public Boolean ContainColumn(IEnumerable<JFCGridColumn> Columns, JFCGridColumn Column)
        {

            // on boucle sur les colonne pour vérifier si on est en multi header
            foreach (JFCGridColumn col in Columns)
            {
                if (col == Column)
                    return true;

                if (col.ChildrenColumns.Count() > 0)
                {
                    return ContainColumn(col.ChildrenColumns, Column);
                }
            }

            return false;
        }

        public UIElement cloneElement(UIElement orig)
        {
            if (orig == null)
                return (null);

            string s = XamlWriter.Save(orig);

            StringReader stringReader = new StringReader(s);

            XmlReader xmlReader = XmlTextReader.Create(stringReader, new XmlReaderSettings());

            return (UIElement)XamlReader.Load(xmlReader);
        }

        internal GridLength CalculTailleHeaderMaxi(JFCGridColumn col)
        {
            Double width = 0.0;
            //Double widthMax = 0.0;
            Size tailleMax = new Size(1000, 1000);

            JFCGridColumnHeader header = new JFCGridColumnHeader();
            Object ob = col.Header.Content;
            col.Header.ContentChanged -= Parent.Header_ContentChanged;
            col.Header.Content = null;
            header.Content = ob;
            header.Template = col.Header.Template;

            header.Measure(tailleMax);

            Size sizeh = header.DesiredSize;

            //UIElement obj = cloneElement(col.Header);
            //JFCGridColumnHeader header = obj as JFCGridColumnHeader;
            //header.Template = col.Header.Template;
            //header.Measure(tailleMax);
            //Size sizeh = header.DesiredSize;

            //col.Header.Measure(tailleMax);
            //Size sizeh = col.Header.DesiredSize;

            width = Math.Ceiling(sizeh.Width);

            //if (widthMax < width)
            //    widthMax = width;

            header.Content = null;
            header = null;

            col.Header.Content = ob;
            col.Header.ContentChanged += new DependencyPropertyChangedEventHandler(Parent.Header_ContentChanged);

            return new GridLength(width);
        }

        private GridLength CalculTailleMaxi(JFCGridColumn col)
        {
            JFCExpendItem[] listData = Parent.dataSource;
            JFCGridColumn mainCol = col;

            bool headerOnly = false;

            if (Parent.dataSource == null)
            {
                if (Parent.dataSourceFooter == null)
                {
                    headerOnly = true;
                    //return new GridLength();
                }
                else if (Parent.dataSourceFooter.Count() == 0)
                {
                    headerOnly = true;
                    //return new GridLength();
                }
                else
                    listData = Parent.dataSourceFooter;
            }
            else if (Parent.dataSource.Count() == 0)
            {
                if (Parent.dataSourceFooter == null)
                {
                    headerOnly = true;
                    //return new GridLength();
                }
                else if (Parent.dataSourceFooter.Count() == 0)
                {
                    headerOnly = true;
                    //return new GridLength();
                }
                else
                    listData = Parent.dataSourceFooter;
            }

            Double width = 0.0;
            Double widthMax = 0.0;
            Size tailleMax = new Size(1000, 1000);

            if (headerOnly == false)
            {
                CellImg cell = new CellImg();
                JFCGridCell cellTemplate = new JFCGridCell();
                HierarchicalItem hitem = new HierarchicalItem();
                JFCGridCellGrouping gitem = new JFCGridCellGrouping();
                MarginConverter margin = new MarginConverter();

                int indexData = 0;

                foreach (var obj in listData.Take(100))
                {
                    if (col.TypeColumn == JFCGridColumn.TypesColumn.Hierarchical)
                    {
                        if (col.CellTemplate == null)
                        {
                            //HierarchicalItem hitem = new HierarchicalItem();

                            hitem.DataContext = obj;

                            object value;
                            if (obj != null && col.GetCellValue(obj.Obj, out value))
                            {
                                if (value != null)
                                    cell.Text = value.ToString();
                                else
                                    cell.Text = "";
                            }
                            else if (col.BindingPath != "" && col.BindingPath != null)
                            {
                                PropertyInfo PInfo = obj.Obj.GetType().GetProperty(col.BindingPath);

                                if (PInfo != null)
                                {
                                    cell.Text = PInfo.GetValue(obj.Obj, null).ToString();
                                }
                                else
                                {
                                    Binding binding = new Binding(col.BindingPath);
                                    binding.Source = obj.Obj;
                                    cell.SetBinding(CellImg.TextProperty, binding);

                                }
                            }
                            else
                            {
                                cell.Text = obj.Obj.GetType().FullName;
                            }

                            ApplyStyleCell(cell, col, ApplyMode.CalculSize);

                            hitem.Component = cell;

                            hitem.Measure(tailleMax);

                            Size size = hitem.DesiredSize;

                            width = size.Width;

                            //MarginConverter margin = new MarginConverter();
                            width += ((Thickness)margin.Convert(obj.Level, null, null, null)).Left;

                            if (widthMax < width)
                                widthMax = width;

                            hitem.Component = null;
                        }
                        else
                        {
                            //HierarchicalItem hitem = new HierarchicalItem();
                            hitem = new HierarchicalItem();
                            cellTemplate = new JFCGridCell();

                            // on réinitialise le composant
                            //cellTemplate.Template = null;
                            //hitem.Component = null;
                            //cellTemplate.DataContext = null;

                            cellTemplate.Template = col.CellTemplate;

                            hitem.DataContext = obj;

                            object value;
                            if (obj != null && col.GetCellValue(obj.Obj, out value))
                            {
                                cellTemplate.DataContext = value;
                            }
                            else if (col.BindingPath != "" && col.BindingPath != null && obj != null)
                            {
                                PropertyInfo data = obj.Obj.GetType().GetProperty(col.BindingPath);

                                if (data != null)
                                {
                                    cellTemplate.DataContext = data.GetValue(obj.Obj, null);
                                }
                                else
                                {
                                    Binding binding = new Binding(col.BindingPath);
                                    binding.Source = obj.Obj;
                                    cellTemplate.SetBinding(JFCGridCell.DataContextProperty, binding);
                                }
                            }
                            else
                            {
                                if (obj == null)
                                    cellTemplate.DataContext = null;
                                else
                                    cellTemplate.DataContext = obj.Obj;
                            }

                            //cellTemplate.InvalidateVisual();

                            //cellTemplate.Measure(tailleMax);
                            //Size sCell = cellTemplate.DesiredSize;

                            //Size rCell = cellTemplate.RenderSize;

                            //hitem.UpdateLayout();

                            hitem.Component = cellTemplate;

                            //hitem.InvalidateVisual();

                            hitem.Measure(tailleMax);

                            Size size = hitem.DesiredSize;

                            width = size.Width;

                            //MarginConverter margin = new MarginConverter();
                            width += ((Thickness)margin.Convert(obj.Level, null, null, null)).Left;

                            if (widthMax < width)
                                widthMax = width;

                            hitem.Component = null;
                        }
                    }
                    else if (col.TypeColumn == JFCGridColumn.TypesColumn.GroupingNoRow || col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRow || col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRowWhenNoChildren)
                    {
                        JFCExpendItem dataContextTmp = obj;

                        //UpdateCellGroupingSizeAndVisibility(gitem,,datacontext);

                        if (dataContextTmp.StartChildViewIndex == indexData || Parent.ScrollVerticalValue == indexData)
                        {
                            gitem = new JFCGridCellGrouping();

                            if (col.CellTemplate == null)
                            {
                                //JFCGridCellGrouping gitem = new JFCGridCellGrouping();                            

                                //JFCExpendItem dataContextTmp = obj;

                                if (col.LevelGrouping >= 0)
                                {
                                    while (dataContextTmp != null)
                                    {
                                        if (dataContextTmp.Level == col.LevelGrouping)
                                        {
                                            break;
                                        }

                                        dataContextTmp = dataContextTmp.Parent;
                                    }
                                }
                                else
                                {
                                    dataContextTmp = null;
                                }

                                if (dataContextTmp != null)
                                {
                                    gitem.DataContext = dataContextTmp;

                                    object value;
                                    if (obj != null && col.GetCellValue(dataContextTmp.Obj, out value))
                                    {
                                        if (value != null)
                                            cell.Text = value.ToString();
                                        else
                                            cell.Text = "";
                                    }
                                    else if (col.BindingPath != "" && col.BindingPath != null)
                                    {
                                        PropertyInfo PInfo = dataContextTmp.Obj.GetType().GetProperty(col.BindingPath);

                                        if (PInfo != null)
                                        {
                                            cell.Text = PInfo.GetValue(dataContextTmp.Obj, null).ToString();
                                        }
                                        else
                                        {
                                            Binding binding = new Binding(col.BindingPath);
                                            binding.Source = dataContextTmp.Obj;
                                            cell.SetBinding(CellImg.TextProperty, binding);

                                        }
                                    }
                                    else
                                    {
                                        cell.Text = dataContextTmp.Obj.GetType().FullName;
                                    }

                                    ApplyStyleCell(cell, col, ApplyMode.CalculSize);

                                    gitem.Component = cell;

                                    gitem.Measure(tailleMax);

                                    Size size = gitem.DesiredSize;

                                    width = size.Width;

                                    if (widthMax < width)
                                        widthMax = width;

                                    gitem.Component = null;
                                }
                            }
                            else
                            {
                                //JFCGridCellGrouping gitem = new JFCGridCellGrouping();

                                // on réinitialise le composant
                                cellTemplate.Template = null;
                                gitem.Component = null;

                                cellTemplate.Template = col.CellTemplate;

                                //JFCExpendItem dataContextTmp = obj;

                                if (col.LevelGrouping >= 0)
                                {
                                    while (dataContextTmp != null)
                                    {
                                        if (dataContextTmp.Level == col.LevelGrouping)
                                        {
                                            break;
                                        }

                                        dataContextTmp = dataContextTmp.Parent;
                                    }
                                }
                                else
                                {
                                    dataContextTmp = null;
                                }

                                if (dataContextTmp != null)
                                {
                                    gitem.DataContext = dataContextTmp;

                                    //cellTemplate.DataContext = obj.Obj;

                                    object value;
                                    if (obj != null && col.GetCellValue(dataContextTmp.Obj, out value))
                                    {
                                        cellTemplate.DataContext = value;
                                    }
                                    else if (col.BindingPath != "" && col.BindingPath != null && dataContextTmp != null)
                                    {
                                        PropertyInfo data = dataContextTmp.Obj.GetType().GetProperty(col.BindingPath);

                                        if (data != null)
                                        {
                                            cellTemplate.DataContext = data.GetValue(dataContextTmp.Obj, null);
                                        }
                                        else
                                        {
                                            Binding binding = new Binding(col.BindingPath);
                                            binding.Source = dataContextTmp.Obj;
                                            cellTemplate.SetBinding(JFCGridCell.DataContextProperty, binding);
                                        }
                                    }
                                    else
                                    {
                                        if (obj == null)
                                            cellTemplate.DataContext = null;
                                        else
                                            cellTemplate.DataContext = dataContextTmp.Obj;
                                    }


                                    gitem.Component = cellTemplate;

                                    gitem.Measure(tailleMax);

                                    Size size = gitem.DesiredSize;

                                    width = size.Width;

                                    if (widthMax < width)
                                        widthMax = width;

                                    gitem.Component = null;
                                }
                            }
                        }
                    }
                    else if (col.CellTemplate == null)
                    {
                        ApplyStyleCell(cell, col, ApplyMode.CalculSize);

                        object value;
                        if (obj != null && col.GetCellValue(obj.Obj, out value))
                        {
                            if (value != null)
                                cell.Text = value.ToString();
                            else
                                cell.Text = "";
                        }
                        else if (col.BindingPath != "" && col.BindingPath != null)
                        {
                            PropertyInfo PInfo = obj.Obj.GetType().GetProperty(col.BindingPath);

                            if (PInfo != null)
                            {
                                cell.Text = PInfo.GetValue(obj.Obj, null).ToString();
                            }
                            else
                            {
                                Binding binding = new Binding(col.BindingPath);
                                binding.Source = obj.Obj;
                                cell.SetBinding(CellImg.TextProperty, binding);
                            }
                        }
                        else
                        {
                            cell.Text = obj.Obj.GetType().FullName;
                        }

                        //ApplyStyleCell(cell, col, ApplyMode.CalculSize);

                        cell.Measure(tailleMax);

                        Size size = cell.DesiredSize;

                        width = size.Width;

                        if (widthMax < width)
                            widthMax = width;
                    }
                    else
                    {
                        // on reinitialise le composant
                        cellTemplate.DataContext = null;
                        cellTemplate.Template = null;

                        cellTemplate.Template = col.CellTemplate;

                        object value;
                        if (obj != null && col.GetCellValue(obj.Obj, out value))
                        {
                            cellTemplate.DataContext = value;
                        }
                        else if (col.BindingPath != "" && col.BindingPath != null)
                        {
                            PropertyInfo data = obj.Obj.GetType().GetProperty(col.BindingPath);

                            if (data != null)
                            {
                                cellTemplate.DataContext = data.GetValue(obj.Obj, null);
                            }
                            else
                            {
                                Binding binding = new Binding(col.BindingPath);
                                binding.Source = obj.Obj;
                                cellTemplate.SetBinding(JFCGridCell.DataContextProperty, binding);
                            }
                        }
                        else
                        {
                            cellTemplate.DataContext = obj.Obj;
                        }

                        cellTemplate.Measure(tailleMax);

                        Size size = cellTemplate.DesiredSize;

                        width = size.Width;

                        if (widthMax < width)
                            widthMax = width;
                    }

                    indexData++;
                }

                widthMax += col.CellPadding.Left + col.CellPadding.Right;

                // on calcul la taille du header

                //HorizontalAlignment oldAlignement = col.Header.HorizontalAlignment;
                //col.Header.HorizontalAlignment = HorizontalAlignment.Left;
                //col.Header.Width = 0;
                //col.Header.Width = double.NaN;
                ////int indexof;

                ////if (col.Frozen)
                ////{
                ////    indexof = Grid00.Children.IndexOf(col.Header);
                ////    Grid00.Children.Remove(col.Header);
                ////}
                ////else
                ////{
                ////    indexof = Grid01.Children.IndexOf(col.Header);
                ////    Grid01.Children.Remove(col.Header);
                ////}

                ////HorizontalAlignment oldAlignement = col.Header.HorizontalAlignment;
                ////col.Header.HorizontalAlignment = HorizontalAlignment.Left;

                ////col.Header.Width = 0;
                ////col.Header.UpdateLayout();
                ////col.Header.Width = double.NaN;
                ////col.Header.UpdateLayout();

                ////col.Header.Measure(tailleMax);

                ////Size sizeh = col.Header.DesiredSize;

                ////width = sizeh.Width;

                ////if (widthMax < width)
                ////    widthMax = width;

                //Parent.TotalGrid.Children.Remove(header);

                //col.Header.HorizontalAlignment = oldAlignement;
                //col.Header.Width = double.NaN;

                ////if (col.Frozen)
                ////{
                ////    Grid00.Children.Insert(indexof, col.Header);
                ////}
                ////else
                ////{
                ////    Grid01.Children.Insert(indexof, col.Header);
                ////}
            }


            while (col != null)
            {
                //JFCGridColumnHeader header = new JFCGridColumnHeader();
                //Object ob = col.Header.Content;
                //col.Header.Content = null;
                //header.Content = ob;
                //header.Template = col.Header.Template;

                //header.Measure(tailleMax);

                //Size sizeh = header.DesiredSize;

                //width = sizeh.Width;


                //UIElement obj = cloneElement(col.Header);
                //JFCGridColumnHeader header = obj as JFCGridColumnHeader;
                //header.Template = col.Header.Template;
                //header.Measure(tailleMax);
                //Size sizeh = header.DesiredSize;

                GridLength gl = CalculTailleHeaderMaxi(col);

                width = gl.Value;

                if (widthMax < width)
                    widthMax = width;


                //header.Content = null;
                //header = null;
                //col.Header.Content = ob;

                if (col.Parent != null)
                    if (col.Parent.ChildrenColumns.Count() == 1)
                    {
                        col = col.Parent;
                    }
                    else
                        col = null;
                else
                    col = null;

            }

            if (mainCol != null)
            {
                if (widthMax <= mainCol.WidthAutoMax)
                    return new GridLength(Math.Ceiling(widthMax));
                else
                    return new GridLength(mainCol.WidthAutoMax);
            }
            else
            {
                return new GridLength(Math.Ceiling(widthMax));
            }
        }

        internal void ResetColumnsWidth()
        {
            foreach (var col in ListColHeaderRow)
            {
                if (col.Width == GridLength.Auto)
                    col.actualWidth = new GridLength(0);
            }

            foreach (var col in ListColHeaderRowEnd)
            {
                if (col.Width == GridLength.Auto)
                    col.actualWidth = new GridLength(0);
            }

            foreach (var col in ListColBody)
            {
                if (col.Width == GridLength.Auto)
                    col.actualWidth = new GridLength(0);
            }

        }

        void Split_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            GridSplitter Split = sender as GridSplitter;

            JFCGridColumn col = Split.Tag as JFCGridColumn;

            col.ActualWidth = CalculTailleMaxi(col);
        }

        void Split_DragCompleted(object sender, System.Windows.Controls.Primitives.DragCompletedEventArgs e)
        {
            GridSplitter Split = sender as GridSplitter;

            JFCGridColumn col = Split.Tag as JFCGridColumn;

            col.Width = col.ActualWidth;

            if (Parent.IsVirtualised == true)
                NbColView = Parent.NbColumnAffichable();
        }

        void Header_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (Parent.ColumnMovable)
            {
                JFCGridColumnHeader header = sender as JFCGridColumnHeader;
                JFCGridColumn col = header.Column;

                if (col.IsMovable)
                {
                    //if (col.Parent == null)
                    //{
                    VisualBrush brush = new VisualBrush(header);

                    //HeaderMove = new Rectangle();

                    HeaderMove.Fill = brush;

                    PtHeaderMove = e.GetPosition(header);

                    HeaderMove.Width = header.ActualWidth;

                    //HeaderMoveFlag.Fill = Brushes.Red;
                    //HeaderMoveFlag.HorizontalAlignment = HorizontalAlignment.Left;
                    //HeaderMoveFlag.VerticalAlignment = VerticalAlignment.Stretch;
                    //HeaderMoveFlag.Width = 2;
                    ////HeaderMoveFlag.Visibility = Visibility.Collapsed;

                    Grid parent = null;

                    var colsFrozenStart = from c in Parent.ColunmDefinition
                                          where c.Frozen == JFCGridColumn.FrozenType.Start
                                          select c;

                    var colsFrozenEnd = from c in Parent.ColunmDefinition
                                        where c.Frozen == JFCGridColumn.FrozenType.End
                                        select c;

                    List<JFCGridColumn> lcolsFrozenStart = colsFrozenStart.ToList();
                    List<JFCGridColumn> lcolsFrozenEnd = colsFrozenEnd.ToList();

                    //if (ListColHeaderRow.Contains(header.Col))
                    if (ContainColumn(lcolsFrozenStart, header.Column))
                    {
                        parent = Grid00;
                    }
                    else if (ContainColumn(lcolsFrozenStart, header.Column))
                    {
                        parent = Grid02;
                    }
                    //else if (ListColBody.Contains(header.Col))
                    else
                    {
                        parent = Grid01;
                    }

                    int idxRow = (int)header.GetValue(Grid.RowProperty);
                    int RowSpan = (int)header.GetValue(Grid.RowSpanProperty);
                    int idxCol = 0;
                    int idxColFlag = 0;
                    int ColSpan = 0;

                    //parent.Children.Add(ColDestinationIndicator);
                    //ColDestinationIndicator.SetValue(Grid.RowProperty, idxRow);
                    //ColDestinationIndicator.SetValue(Grid.RowSpanProperty, RowSpan);

                    if (header.Column.Parent == null)
                    {
                        idxCol = 0;
                    }
                    else
                    {
                        idxCol = (int)header.Column.Parent.Header.GetValue(Grid.ColumnProperty);
                    }

                    idxColFlag = (int)header.GetValue(Grid.ColumnProperty);


                    if (header.Column.Parent == null)
                        ColSpan = parent.ColumnDefinitions.Count();
                    else
                        ColSpan = FindChildrenColumn(header.Column.Parent.ChildrenColumns).Count();

                    //HeaderMove.SetValue(Grid.RowProperty, idxRow);
                    //HeaderMove.SetValue(Grid.ColumnSpanProperty, parent.ColumnDefinitions.Count());

                    HeaderMove.SetValue(Grid.RowProperty, idxRow);
                    HeaderMove.SetValue(Grid.RowSpanProperty, RowSpan);
                    HeaderMove.SetValue(Grid.ColumnProperty, idxCol);
                    HeaderMove.SetValue(Grid.ColumnSpanProperty, ColSpan);
                    HeaderMove.HorizontalAlignment = HorizontalAlignment.Left;


                    //HeaderMoveFlag.SetValue(Grid.RowProperty, idxRow);
                    //HeaderMoveFlag.SetValue(Grid.RowSpanProperty, RowSpan);
                    //HeaderMoveFlag.SetValue(Grid.ColumnProperty, idxColFlag);

                    if (parent != null)
                    {
                        //HeaderMoveFlag.SetValue(Panel.ZIndexProperty, 1000);
                        //parent.Children.Add(HeaderMoveFlag);

                        HeaderMove.SetValue(Panel.ZIndexProperty, 1000);
                        HeaderMove.Opacity = 0.75;

                        //if (parent.Children.Contains(HeaderMove))
                        //    parent.Children.Remove(HeaderMove);

                        if (HeaderMove.Parent != null)
                        {
                            Grid grid = HeaderMove.Parent as Grid;

                            if (grid != null)
                                grid.Children.Remove(HeaderMove);
                        }

                        parent.Children.Add(HeaderMove);
                    }
                    //}
                }
            }
        }

        void Header_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (Parent.ColumnMovable)
            {
                if (HeaderMove != null && HeaderMove.Parent != null)
                {
                    JFCGridColumnHeader header = sender as JFCGridColumnHeader;

                    Grid parent = null;

                    var colsFrozenStart = from c in Parent.ColunmDefinition
                                          where c.Frozen == JFCGridColumn.FrozenType.Start
                                          select c;

                    var colsFrozenEnd = from c in Parent.ColunmDefinition
                                        where c.Frozen == JFCGridColumn.FrozenType.End
                                        select c;

                    List<JFCGridColumn> lcolsFrozenStart = colsFrozenStart.ToList();
                    List<JFCGridColumn> lcolsFrozenEnd = colsFrozenEnd.ToList();

                    JFCGridColumn.FrozenType colFrozen = JFCGridColumn.FrozenType.None;

                    if (ContainColumn(lcolsFrozenStart, header.Column))
                    {
                        parent = Grid00;
                        colFrozen = JFCGridColumn.FrozenType.Start;
                    }
                    if (ContainColumn(lcolsFrozenEnd, header.Column))
                    {
                        parent = Grid02;
                        colFrozen = JFCGridColumn.FrozenType.End;
                    }
                    else
                    {
                        parent = Grid01;
                        colFrozen = JFCGridColumn.FrozenType.None;
                    }

                    //parent.Children.Remove(ColDestinationIndicator);

                    Point pt = e.GetPosition(parent);

                    if (header.Column.Parent != null)
                    {
                        int c = (int)HeaderMove.GetValue(Grid.ColumnProperty);
                        double taille = 0.0;

                        for (int i = 0; i < c; i++)
                        {
                            taille += parent.ColumnDefinitions[i].Width.Value;
                        }

                        pt.X -= taille;
                    }

                    if (parent != null)
                    {
                        parent.Children.Remove(HeaderMove);
                    }

                    //HeaderMove = null;
                    //PtHeaderMove = null;

                    int idxCol = 0;
                    int idxColBody = 0;
                    double t = 0;
                    int idxColPos = 0;

                    ObservableCollection<JFCGridColumn> lstCol;

                    if (header.Column.Parent == null)
                    {
                        lstCol = Parent.ColunmDefinition;
                    }
                    else
                    {
                        // TODO pour gérer les mouvements des colonnes en multi header
                        return;
                        lstCol = header.Column.Parent.ChildrenColumns;
                    }

                    idxColPos = lstCol.IndexOf(header.Column);

                    if (idxColPos >= 0)
                    {

                        foreach (var c in lstCol)
                        {
                            if (c.Frozen == header.Column.Frozen)
                            {
                                if (colFrozen == JFCGridColumn.FrozenType.None)
                                {
                                    if (idxColBody < startIndexColView)
                                    {

                                        int colSpan = CalculColSpan(c);

                                        if (idxColBody + colSpan - 1 < startIndexColView)
                                        {
                                            idxColBody += colSpan;
                                            idxCol++;
                                        }
                                        else
                                        {

                                            //idxColBody += colSpan;

                                            var lstChild = FindChildrenColumn(c.ChildrenColumns);

                                            for (int i = 0; i < colSpan; i++)
                                            {
                                                if (startIndexColView <= idxColBody)
                                                {
                                                    t += lstChild[i].ActualWidth.Value;
                                                }

                                                idxColBody++;
                                            }

                                            idxCol++;
                                        }

                                        continue;
                                    }
                                }

                                if (idxCol < idxColPos)
                                {
                                    if (t < pt.X && pt.X < t + c.ActualWidth.Value)
                                    {
                                        if (pt.X < (t + (c.ActualWidth.Value / 2)))
                                        {
                                            lstCol.Move(idxColPos, idxCol);
                                            break;
                                        }
                                        else
                                        {
                                            lstCol.Move(idxColPos, idxCol + 1);
                                            break;
                                        }
                                    }
                                }
                                else if (idxColPos < idxCol)
                                {
                                    if (t < pt.X && pt.X < t + c.ActualWidth.Value)
                                    {
                                        if (pt.X < (t + (c.ActualWidth.Value / 2)))
                                        {
                                            lstCol.Move(idxColPos, idxCol - 1);
                                            break;
                                        }
                                        else
                                        {
                                            lstCol.Move(idxColPos, idxCol);
                                            break;
                                        }
                                    }
                                }

                                t += c.ActualWidth.Value;
                            }
                            idxCol++;

                            if (c.Frozen == JFCGridColumn.FrozenType.None)
                                idxColBody++;
                        }
                    }
                }
            }
        }

        void Header_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (HeaderMove != null && HeaderMove.Parent != null && e.LeftButton == MouseButtonState.Pressed)
            {
                JFCGridColumnHeader header = sender as JFCGridColumnHeader;
                JFCGridColumn col = header.Column;

                Grid parentH = null;
                Grid parentB = null;

                var colsFrozenStart = from c in Parent.ColunmDefinition
                                      where c.Frozen == JFCGridColumn.FrozenType.Start
                                      select c;

                var colsFrozenEnd = from c in Parent.ColunmDefinition
                                    where c.Frozen == JFCGridColumn.FrozenType.End
                                    select c;

                List<JFCGridColumn> lcolsFrozenStart = colsFrozenStart.ToList();
                List<JFCGridColumn> lcolsFrozenEnd = colsFrozenEnd.ToList();

                //if (ListColHeaderRow.Contains(header.Col))
                if (ContainColumn(lcolsFrozenStart, header.Column))
                {
                    parentH = Grid00;
                    parentB = Grid10;
                }
                else if (ContainColumn(lcolsFrozenEnd, header.Column))
                {
                    parentH = Grid02;
                    parentB = Grid12;
                }
                //else if (ListColBody.Contains(header.Col))
                else
                {
                    parentH = Grid01;
                    parentB = Grid11;
                }

                if (parentH != null)
                {
                    Point pt = e.GetPosition(parentB);

                    if (col.Parent != null)
                    {
                        int c = (int)HeaderMove.GetValue(Grid.ColumnProperty);
                        double taille = 0.0;

                        for (int i = 0; i < c; i++)
                        {
                            taille += parentH.ColumnDefinitions[i].Width.Value;
                        }

                        pt.X -= taille;
                    }

                    HeaderMove.Margin = new Thickness(pt.X - PtHeaderMove.X, 0, 0, 0);
                }
            }
            else
            {
                Grid grid = HeaderMove.Parent as Grid;

                if (grid != null)
                    grid.Children.Remove(HeaderMove);
            }
        }

        private void ClearParentLstRemoveCol()
        {
            foreach (var cell in LstRemoveColCellImg)
            {
                if (cell.Parent is Grid)
                {
                    ((Grid)cell.Parent).Children.Remove(cell);
                    cell.Column = null;
                    DeleteEvents(cell);
                }
            }

            foreach (var cell in LstRemoveColJFCGridCell)
            {
                if (cell.Parent is Grid)
                {
                    ((Grid)cell.Parent).Children.Remove(cell);
                    DeleteEvents(cell);
                }
            }

        }

        private void ClearLstRemoveCol()
        {
            ClearParentLstRemoveCol();

            LstRecycleCellImg.AddRange(LstRemoveColCellImg);
            LstRemoveColCellImg.Clear();

            LstRecycleJFCGridCell.AddRange(LstRemoveColJFCGridCell);
            LstRemoveColJFCGridCell.Clear();

        }

        #endregion

        #region Rows manager

        public void RowAdd(Boolean UpdateData)
        {
            int Index = Grid11.RowDefinitions.Count();

            if (RowAdd(Index))
                if (UpdateData)
                    RowRefreshData(Index);
        }

        public bool RowAdd(int Index)
        {
            if (Parent.dataSource == null)
                return false;

            int IndexData = Parent.ScrollVerticalValue + Index;

            if (IndexData >= Parent.dataSource.Count())
                return false;

            JFCExpendItem dataContext = Parent.dataSource[IndexData];

            JFCGridItem itemH = new JFCGridItem(null);
            JFCGridItem itemB = new JFCGridItem(null);
            JFCGridItem itemF = new JFCGridItem(null);

            itemH.Peer.Add(itemB);
            itemH.Peer.Add(itemF);

            itemB.Peer.Add(itemH);
            itemB.Peer.Add(itemF);

            itemF.Peer.Add(itemH);
            itemF.Peer.Add(itemB);

            itemH.Orientation = Orientation.Horizontal;
            itemB.Orientation = Orientation.Horizontal;
            itemF.Orientation = Orientation.Horizontal;

            Grid.SetZIndex(itemH, -12);
            Grid.SetZIndex(itemB, -12);
            Grid.SetZIndex(itemF, -12);

            RowAddGrid(Grid10, itemH, Index, IndexData, TabLineH, dataContext, ListColHeaderRow);

            // création de la row du body
            RowAddGridVirtual(Grid11, itemB, Index, IndexData, TabLineB, dataContext, ListColBody);

            RowAddGrid(Grid12, itemF, Index, IndexData, TabLineF, dataContext, ListColHeaderRowEnd);

            return true;
        }

        public void RowFooterAdd(Boolean UpdateData)
        {
            int Index = Grid21.RowDefinitions.Count();
            RowFooterAdd(Index);

            if (UpdateData)
                RowFooterRefreshData(Index);
        }

        public void RowFooterAdd(int Index)
        {

            if (Parent.dataSourceFooter == null)
                return;

            int IndexData = Index;

            if (IndexData >= Parent.dataSourceFooter.Count())
                return;

            JFCExpendItem dataContext = Parent.dataSourceFooter[IndexData];

            JFCGridItem itemHF = new JFCGridItem(null);
            JFCGridItem itemBF = new JFCGridItem(null);

            itemHF.Peer.Add(itemBF);
            itemBF.Peer.Add(itemHF);

            itemHF.Orientation = Orientation.Horizontal;
            itemBF.Orientation = Orientation.Horizontal;

            Grid.SetZIndex(itemHF, -1);
            Grid.SetZIndex(itemBF, -1);

            RowAddGrid(Grid20, itemHF, Index, IndexData, TabLineHF, dataContext, ListColHeaderFooter);

            // création de la row du body
            RowAddGridVirtual(Grid21, itemBF, Index, IndexData, TabLineBF, dataContext, ListColBody);
        }

        private void RowAddGrid(Grid grid, JFCGridItem item, int Index, int IndexData, List<LinkedList<UIElement>> TabLineGrid, JFCExpendItem DataContext, List<JFCGridColumn> ListCol)
        {
            //////////////////////////////////////////////
            // on ajoute la ligne dans le grid

            RowDefinition rowDefBody = new RowDefinition();
            rowDefBody.Height = new GridLength(Parent.RowHeight);
            grid.RowDefinitions.Insert(Index, rowDefBody);
            int NbRow = grid.RowDefinitions.Count();


            LinkedList<UIElement> lUIElement = new LinkedList<UIElement>();
            lUIElement.AddLast(item);

            // on rallonge la ligne de séparation des colonnes
            IEnumerable<UIElement> ListCtrl = grid.Children.Cast<UIElement>();
            var ctrls = from ctrl in ListCtrl
                        where ctrl is Line || ctrl is JFCGridItem
                        select ctrl;

            foreach (var ctrl in ctrls)
            {
                if (ctrl is Line)
                {
                    Line l = ctrl as Line;
                    if (l.Orientation == Orientation.Vertical)
                    {
                        ctrl.SetValue(Grid.RowProperty, 0);
                        ctrl.SetValue(Grid.RowSpanProperty, NbRow);
                    }
                }
                else if (ctrl is JFCGridItem)
                {
                    JFCGridItem l = ctrl as JFCGridItem;
                    if (l.Orientation == Orientation.Vertical)
                    {
                        ctrl.SetValue(Grid.RowProperty, 0);
                        ctrl.SetValue(Grid.RowSpanProperty, NbRow);
                    }
                }
            }

            for (int idxR = Index; idxR < TabLineGrid.Count(); idxR++)
            {
                foreach (var ctrl in TabLineGrid[idxR])
                {
                    if (ctrl is Line)
                    {
                        Line l = ctrl as Line;
                        if (l.Orientation != Orientation.Vertical)
                        {
                            //idxR = (int)ctrl.GetValue(Grid.RowProperty);
                            //idxR++;
                            ctrl.SetValue(Grid.RowProperty, idxR + 1);
                        }
                    }
                    else if (ctrl is JFCGridItem)
                    {
                        JFCGridItem l = ctrl as JFCGridItem;
                        if (l.Orientation != Orientation.Vertical)
                        {
                            //idxR = (int)ctrl.GetValue(Grid.RowProperty);
                            //idxR++;
                            ctrl.SetValue(Grid.RowProperty, idxR + 1);
                        }
                    }
                    else
                    {
                        //idxR = (int)ctrl.GetValue(Grid.RowProperty);
                        idxR++;
                        ctrl.SetValue(Grid.RowProperty, idxR + 1);
                    }
                }
            }

            // on ajoute un JFCGridItem            
            item.Data = DataContext;
            item.IndexData = IndexData;
            //item.MouseLeftButtonDown += new System.Windows.Input.MouseButtonEventHandler(item_MouseLeftButtonDown);
            //item.MouseLeftButtonUp += new System.Windows.Input.MouseButtonEventHandler(item_MouseLeftButtonUp);
            //item.MouseRightButtonUp += new MouseButtonEventHandler(item_MouseRightButtonUp);
            item.IsMouseOverLineChanged += new RoutedEventHandler(item_IsMouseOverLineChanged);
            item.ContextMenu = Parent.ContextmenuRow;
            item.BorderBrush = Parent.HorizontalBorderColor;
            item.SetValue(Grid.ColumnProperty, 0);
            if (ListCol.Count() > 0)
                item.SetValue(Grid.ColumnSpanProperty, ListCol.Count());

            item.SetValue(Grid.RowProperty, Index);

            SetupEvents(item);

            if ((IndexData % 2) == 0)
                item.SetValue(JFCGridItem.IsAlternateProperty, true);

            grid.Children.Add(item);

            int idxCol = 0;
            //CellImg cell;
            //JFCGridCell cellTemplate;

            //List<FrameworkElement> lstCell = new List<FrameworkElement>();

            // on boucle sur les colonnes pour ajouter les cellules
            foreach (var Col in ListCol)
            {
                SetupCellData(grid, lUIElement, Col, item, idxCol, Index, DataContext);

                idxCol++;
            }

            // on ajoute une ligne de séparation
            Line bdG = new Line();

            bdG.LineBrush = Parent.HorizontalBorderColor;

            bdG.Thickness = 1;
            bdG.Orientation = Orientation.Horizontal;
            bdG.VerticalAlignment = VerticalAlignment.Bottom;

            bdG.ContextMenu = item.ContextMenu;

            bdG.SetValue(Grid.RowProperty, Index);
            if (ListCol.Count() > 0)
                bdG.SetValue(Grid.ColumnSpanProperty, ListCol.Count());

            SetupEvents(bdG);

            // on ajoute la ligne
            grid.Children.Add(bdG);
            lUIElement.AddLast(bdG);

            TabLineGrid.Insert(Index, lUIElement);
        }

        private void RowAddGridVirtual(Grid grid, JFCGridItem item, int Index, int IndexData, List<LinkedList<UIElement>> TabLineGrid, JFCExpendItem DataContext, IList<JFCGridColumn> ListCol)
        {
            //////////////////////////////////////////////
            // on ajoute la ligne dans le grid

            RowDefinition rowDefBody = new RowDefinition();
            rowDefBody.Height = new GridLength(Parent.RowHeight);
            grid.RowDefinitions.Insert(Index, rowDefBody);
            int NbRow = grid.RowDefinitions.Count();


            LinkedList<UIElement> lUIElement = new LinkedList<UIElement>();
            lUIElement.AddLast(item);

            // on rallonge la ligne de séparation des colonnes
            IEnumerable<UIElement> ListCtrl = grid.Children.Cast<UIElement>();
            var ctrls = from ctrl in ListCtrl
                        where ctrl is Line || ctrl is JFCGridItem
                        select ctrl;

            foreach (var ctrl in ctrls)
            {
                if (ctrl is Line)
                {
                    Line l = ctrl as Line;
                    if (l.Orientation == Orientation.Vertical)
                    {
                        ctrl.SetValue(Grid.RowProperty, 0);
                        ctrl.SetValue(Grid.RowSpanProperty, NbRow);
                    }
                }
                else if (ctrl is JFCGridItem)
                {
                    JFCGridItem l = ctrl as JFCGridItem;
                    if (l.Orientation == Orientation.Vertical)
                    {
                        ctrl.SetValue(Grid.RowProperty, 0);
                        ctrl.SetValue(Grid.RowSpanProperty, NbRow);
                    }
                }
            }

            for (int idxR = Index; idxR < TabLineGrid.Count(); idxR++)
            {
                foreach (var ctrl in TabLineGrid[idxR])
                {
                    if (ctrl is Line)
                    {
                        Line l = ctrl as Line;
                        if (l.Orientation != Orientation.Vertical)
                        {
                            //idxR = (int)ctrl.GetValue(Grid.RowProperty);
                            //idxR++;
                            ctrl.SetValue(Grid.RowProperty, idxR + 1);
                        }
                    }
                    else if (ctrl is JFCGridItem)
                    {
                        JFCGridItem l = ctrl as JFCGridItem;
                        if (l.Orientation != Orientation.Vertical)
                        {
                            //idxR = (int)ctrl.GetValue(Grid.RowProperty);
                            //idxR++;
                            ctrl.SetValue(Grid.RowProperty, idxR + 1);
                        }
                    }
                    else
                    {
                        //idxR = (int)ctrl.GetValue(Grid.RowProperty);
                        idxR++;
                        ctrl.SetValue(Grid.RowProperty, idxR + 1);
                    }
                }
            }

            // on ajoute un JFCGridItem            
            item.Data = DataContext;
            item.IndexData = IndexData;
            //item.MouseLeftButtonDown += new System.Windows.Input.MouseButtonEventHandler(item_MouseLeftButtonDown);
            //item.MouseLeftButtonUp += new System.Windows.Input.MouseButtonEventHandler(item_MouseLeftButtonUp);
            //item.MouseRightButtonUp += new MouseButtonEventHandler(item_MouseRightButtonUp);
            item.IsMouseOverLineChanged += new RoutedEventHandler(item_IsMouseOverLineChanged);
            item.ContextMenu = Parent.ContextmenuRow;
            item.BorderBrush = Parent.HorizontalBorderColor;
            item.SetValue(Grid.ColumnProperty, 0);
            if (grid.ColumnDefinitions.Count() > 0)
            {
                //item.SetValue(Grid.ColumnSpanProperty, ListCol.Count());
                //item.SetValue(Grid.ColumnSpanProperty, NbColView);
                item.SetValue(Grid.ColumnSpanProperty, grid.ColumnDefinitions.Count());
            }
            item.SetValue(Grid.RowProperty, Index);

            SetupEvents(item);

            if ((IndexData % 2) == 0)
                item.SetValue(JFCGridItem.IsAlternateProperty, true);

            grid.Children.Add(item);

            int idxCol = 0;
            int nbCol = ListCol.Count();

            // on boucle sur les colonnes pour ajouter les cellules
            //foreach (var Col in ListCol)
            for (int i = StartIndexColView; i <= (StartIndexColView + grid.ColumnDefinitions.Count() - 1); i++)
            {
                if (i < nbCol)
                {
                    JFCGridColumn Col = ListCol[i];
                    SetupCellData(grid, lUIElement, Col, item, idxCol, Index, DataContext);

                    idxCol++;
                }
                else
                {
                    throw new Exception("Nombre de colonnes du grid plus importante que les colonnes déclarées dans la JFCGrid");
                }
            }

            // on ajoute une ligne de séparation
            Line bdG = new Line();

            bdG.LineBrush = Parent.HorizontalBorderColor;

            bdG.Thickness = 1;
            bdG.Orientation = Orientation.Horizontal;
            bdG.VerticalAlignment = VerticalAlignment.Bottom;

            bdG.ContextMenu = item.ContextMenu;

            bdG.SetValue(Grid.RowProperty, Index);
            if (grid.ColumnDefinitions.Count() > 0)
            {
                bdG.SetValue(Grid.ColumnSpanProperty, grid.ColumnDefinitions.Count());
            }

            SetupEvents(bdG);

            // on ajoute la ligne
            grid.Children.Add(bdG);
            lUIElement.AddLast(bdG);

            TabLineGrid.Insert(Index, lUIElement);
        }

        private void SetupCellData(Grid grid, LinkedList<UIElement> lUIElement, JFCGridColumn Col, JFCGridItem Item, int IdxCol, int IdxRow, JFCExpendItem DataContext)
        {
            CellImg cell;
            JFCGridCell cellTemplate;
            bool removeCol = false;

            if (Col.CellTemplate == null)
            {
                if (LstRemoveColCellImg.Count() > 0 && LstRemoveColCellImg.Last().Parent == grid)
                {
                    cell = LstRemoveColCellImg.Last();
                    LstRemoveColCellImg.Remove(cell);
                    removeCol = true;
                }
                else if (LstRecycleCellImg.Count() > 0)
                {
                    cell = LstRecycleCellImg.Last();
                    LstRecycleCellImg.RemoveAt(LstRecycleCellImg.Count() - 1);
                }
                else
                {
                    cell = new CellImg();
                }

                cell.Tag = null;

                cell.HorizontalAlignment = HorizontalAlignment.Stretch;
                cell.VerticalAlignment = VerticalAlignment.Stretch;

                //JFCGridColumn column = Col;

                cell.Column = Col;

                cell.PropertyChanged += delegate (object sender, PropertyChangedExtendedEventArgs<Object> e)
                {
                    if (e.PropertyName == "Data")
                    {
                        CellImg c = sender as CellImg;

                        if (c.Data != null)
                        {

                            //if (e.OldValue is INotifyPropertyChanged)
                            //{
                            //    INotifyPropertyChanged nold = e.OldValue as INotifyPropertyChanged;
                            //    nold.PropertyChanged -= n_PropertyChanged;
                            //}

                            object value;

                            if (c.Column.GetCellValue(c.Data, out value))
                            {
                                c.Text = value.ToString();
                                if (c.Data is INotifyPropertyChanged)
                                {
                                    INotifyPropertyChanged n = c.Data as INotifyPropertyChanged;

                                    PropertyChangedEventHandler d = delegate (object sender1, PropertyChangedEventArgs e1)
                                    {
                                        if (e1.PropertyName == c.Column.BindingPath)
                                        {
                                            c.Dispatcher.BeginInvoke(
                                                (Action)delegate ()
                                                {
                                                    object val;
                                                    c.Column.GetCellValue(c.Data, out val);

                                                    if (val != null)
                                                        c.Text = val.ToString();
                                                    else
                                                        c.Text = "";
                                                });
                                        }
                                    };


                                    n.PropertyChanged += delegate (object sender1, PropertyChangedEventArgs e1)
                                    {

                                        c.Dispatcher.BeginInvoke(
                                            (Action)delegate ()
                                            {
                                                if (e1.PropertyName == c.Column.BindingPath)
                                                {
                                                    object val;
                                                    c.Column.GetCellValue(c.Data, out val);

                                                    if (val != null)
                                                        c.Text = val.ToString();
                                                    else
                                                        c.Text = "";
                                                }
                                            });
                                    };
                                }
                            }
                            else
                            {
                                if (c.Column.BindingPath != null)
                                {
                                    PropertyInfo data = c.Data.GetType().GetProperty(c.Column.BindingPath);

                                    if (data != null)
                                    {
                                        if (c.Data is INotifyPropertyChanged)
                                        {
                                            INotifyPropertyChanged n = c.Data as INotifyPropertyChanged;
                                            n.PropertyChanged += delegate (object sender1, PropertyChangedEventArgs e1)
                                            {
                                                if (e1.PropertyName == c.Column.BindingPath)
                                                {
                                                    c.Dispatcher.BeginInvoke(
                                                            (Action)delegate ()
                                                            {
                                                                var val = data.GetValue(c.Data, null);

                                                                if (val != null)
                                                                    c.Text = val.ToString();
                                                                else
                                                                    c.Text = "";
                                                            });
                                                }
                                            };
                                        }

                                        var va = data.GetValue(c.Data, null);

                                        if (va != null)
                                            c.Text = va.ToString();
                                        else
                                            c.Text = "";
                                    }
                                    else
                                    {
                                        //c.Text = "";
                                        Binding binding = new Binding(c.Column.BindingPath);
                                        binding.Source = c.Data;
                                        c.SetBinding(CellImg.TextProperty, binding);
                                    }
                                }
                                else
                                {
                                    //c.Text = "";
                                    Binding binding = new Binding();
                                    binding.Source = c.Data;
                                    c.SetBinding(CellImg.TextProperty, binding);
                                }
                            }
                        }
                        else
                        {
                            c.Text = "";
                        }
                    }
                };

                cell.SetValue(Grid.ColumnProperty, IdxCol);
                cell.SetValue(Grid.RowProperty, IdxRow);

                ApplyStyleCell(cell, Col);

                SetupEvents(cell);

                cell.ContextMenu = Item.ContextMenu;

                if (Col.TypeColumn == JFCGridColumn.TypesColumn.Normal)
                {
                    // on ajoute les céllules
                    if (removeCol == false)
                    {
                        if (cell.Parent is Grid)
                            ((Grid)cell.Parent).Children.Remove(cell);

                        grid.Children.Add(cell);
                    }

                    lUIElement.AddLast(cell);
                }
                else if (Col.TypeColumn == JFCGridColumn.TypesColumn.Hierarchical)
                {
                    HierarchicalItem hi = new HierarchicalItem();
                    hi.Component = cell;
                    //hi.DataContext = Parent.dataSource[IndexData];
                    hi.DataContext = DataContext;

                    DeleteEvents(cell);

                    hi.SetValue(Grid.ColumnProperty, IdxCol);
                    hi.SetValue(Grid.RowProperty, IdxRow);

                    SetupEvents(hi);

                    if (removeCol == true)
                        grid.Children.Remove(cell);

                    grid.Children.Add(hi);
                    lUIElement.AddLast(hi);
                }
                else if (Col.TypeColumn == JFCGridColumn.TypesColumn.GroupingNoRow || Col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRow || Col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRowWhenNoChildren)
                {

                    int indexData = Parent.ScrollVerticalValue + IdxRow;

                    JFCExpendItem dataContextTmp = DataContext;

                    if (Col.LevelGrouping >= 0)
                    {
                        while (dataContextTmp != null)
                        {
                            if (dataContextTmp.Level == Col.LevelGrouping)
                            {
                                break;
                            }

                            dataContextTmp = dataContextTmp.Parent;
                        }
                    }
                    else
                    {
                        dataContextTmp = null;
                    }

                    //if (dataContextTmp != null)
                    //{
                    //if (indexData == 0 || dataContextTmp.StartChildIndex == indexData)
                    //{

                    JFCGridCellGrouping cg = new JFCGridCellGrouping();
                    cg.Component = cell;
                    cg.DataContext = dataContextTmp;

                    DeleteEvents(cell, true);

                    if (dataContextTmp != null)
                    {
                        if (Parent.SelectedExpendItems.Contains(dataContextTmp))
                            cg.IsSelected = true;
                    }

                    cg.SetValue(Grid.ColumnProperty, IdxCol);
                    cg.SetValue(Grid.RowProperty, IdxRow);
                    cg.SetValue(Grid.ZIndexProperty, 1000);

                    UpdateCellGroupingSizeAndVisibility(cg, indexData, Col, dataContextTmp);

                    SetupEvents(cg);

                    if (removeCol == true)
                        grid.Children.Remove(cell);

                    // on ajoute les céllules
                    grid.Children.Add(cg);

                    lUIElement.AddLast(cg);
                    //TabCellGrouping.Add(cg);

                    //}
                    //}
                }
            }
            else
            {
                if (LstRemoveColJFCGridCell.Count() > 0 && LstRemoveColJFCGridCell.Last().Parent == grid)
                {
                    cellTemplate = LstRemoveColJFCGridCell.Last();
                    LstRemoveColJFCGridCell.RemoveAt(LstRemoveColJFCGridCell.Count() - 1);
                    removeCol = true;
                }
                else if (LstRecycleJFCGridCell.Count() > 0)
                {
                    cellTemplate = LstRecycleJFCGridCell.Last();
                    LstRecycleJFCGridCell.RemoveAt(LstRecycleJFCGridCell.Count() - 1);
                }
                else
                {
                    cellTemplate = new JFCGridCell();
                }

                cellTemplate.Tag = null;

                cellTemplate.HorizontalAlignment = HorizontalAlignment.Stretch;
                cellTemplate.VerticalAlignment = VerticalAlignment.Stretch;

                cellTemplate.ItemRow = Item;
                cellTemplate.ItemColumn = Col.Item;

                cellTemplate.Template = Col.CellTemplate;

                JFCGridColumn column = Col;

                cellTemplate.SetValue(Grid.ColumnProperty, IdxCol);
                cellTemplate.SetValue(Grid.RowProperty, IdxRow);

                SetupEvents(cellTemplate);

                cellTemplate.ContextMenu = Item.ContextMenu;

                if (column.TypeColumn == JFCGridColumn.TypesColumn.Normal)
                {
                    if (removeCol == false)
                        if (grid != cellTemplate.Parent)
                            grid.Children.Add(cellTemplate);

                    lUIElement.AddLast(cellTemplate);
                }
                else if (column.TypeColumn == JFCGridColumn.TypesColumn.Hierarchical)
                {
                    HierarchicalItem hi = new HierarchicalItem();
                    hi.Component = cellTemplate;
                    ///hi.DataContext = Parent.dataSource[IndexData];
                    hi.DataContext = DataContext;

                    DeleteEvents(cellTemplate);

                    hi.SetValue(Grid.ColumnProperty, IdxCol);
                    hi.SetValue(Grid.RowProperty, IdxRow);

                    SetupEvents(hi);

                    if (removeCol == true)
                        grid.Children.Remove(cellTemplate);

                    grid.Children.Add(hi);
                    lUIElement.AddLast(hi);
                }
                else if (column.TypeColumn == JFCGridColumn.TypesColumn.GroupingNoRow || column.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRow || column.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRowWhenNoChildren)
                {
                    //JFCGridCellGrouping cg = new JFCGridCellGrouping();
                    //cg.Component = cellTemplate;
                    //cg.DataContext = DataContext;

                    //cg.SetValue(Grid.ColumnProperty, IdxCol);
                    //cg.SetValue(Grid.RowProperty, IdxRow);

                    //SetupEvents(cg);

                    //// on ajoute les céllules
                    //grid.Children.Add(cg);

                    //lUIElement.AddLast(cg);
                    //TabCellGrouping.Add(cg);


                    //////////////////////////////////////////
                    //////////////////////////////////////////

                    int indexData = Parent.ScrollVerticalValue + IdxRow;

                    JFCExpendItem dataContextTmp = DataContext;

                    if (Col.LevelGrouping >= 0)
                    {
                        while (dataContextTmp != null)
                        {
                            if (dataContextTmp.Level == Col.LevelGrouping)
                            {
                                break;
                            }

                            dataContextTmp = dataContextTmp.Parent;
                        }
                    }
                    else
                    {
                        dataContextTmp = null;
                    }

                    JFCGridCellGrouping cg = new JFCGridCellGrouping();
                    cg.Component = cellTemplate;
                    cg.DataContext = dataContextTmp;

                    DeleteEvents(cellTemplate);

                    if (dataContextTmp != null)
                    {
                        if (Parent.SelectedExpendItems.Contains(dataContextTmp))
                            cg.IsSelected = true;
                    }

                    cg.SetValue(Grid.ColumnProperty, IdxCol);
                    cg.SetValue(Grid.RowProperty, IdxRow);
                    cg.SetValue(Grid.ZIndexProperty, 1000);

                    UpdateCellGroupingSizeAndVisibility(cg, indexData, column, dataContextTmp);

                    SetupEvents(cg);

                    if (removeCol == true)
                        grid.Children.Remove(cellTemplate);

                    // on ajoute les céllules
                    grid.Children.Add(cg);

                    lUIElement.AddLast(cg);
                    //TabCellGrouping.Add(cg);

                    //}
                    //}

                }
            }
        }

        private void RowMoveTab(List<LinkedList<UIElement>> TabLine, int IndexBefore, int IndexAfter)
        {
            int idxRow = 0;

            // on bouge les autres lignes
            if (IndexBefore < IndexAfter)
            {
                for (int i = IndexBefore; i < IndexAfter; i++)
                {
                    foreach (var ctrl in TabLine[i])
                    {
                        if (ctrl is Line)
                        {
                            Line b = ctrl as Line;
                            if (b.Orientation == Orientation.Horizontal)
                            {
                                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
                                idxRow--;
                                ctrl.SetValue(Grid.RowProperty, idxRow);
                            }
                        }
                        else if (ctrl is JFCGridItem)
                        {
                            // on supprime le hilite
                            MyElement_MouseLeave(ctrl, null);

                            idxRow = (int)ctrl.GetValue(Grid.RowProperty);
                            idxRow--;
                            ctrl.SetValue(Grid.RowProperty, idxRow);
                        }
                        else if (ctrl is JFCGridCellGrouping)
                        {
                            idxRow = (int)ctrl.GetValue(Grid.RowProperty);
                            idxRow--;
                            ctrl.SetValue(Grid.RowProperty, idxRow);

                            //int nbRowAffi = Parent.NbRowAffichable();
                            int indexData = Parent.ScrollVerticalValue + idxRow;

                            JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

                            JFCExpendItem datacontext = cg.DataContext as JFCExpendItem;

                            if (datacontext != null)
                            {
                                UpdateCellGroupingSizeAndVisibility(cg, indexData, null, datacontext);
                            }
                        }
                        else
                        {
                            idxRow = (int)ctrl.GetValue(Grid.RowProperty);
                            idxRow--;
                            ctrl.SetValue(Grid.RowProperty, idxRow);
                        }
                    }
                }
            }
            else if (IndexBefore > IndexAfter)
            {
                for (int i = IndexBefore; i > IndexAfter; i--)
                {
                    foreach (var ctrl in TabLine[i])
                    {
                        if (ctrl is Line)
                        {
                            Line b = ctrl as Line;
                            if (b.Orientation == Orientation.Horizontal)
                            {
                                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
                                idxRow++;
                                ctrl.SetValue(Grid.RowProperty, idxRow);
                            }
                        }
                        else if (ctrl is JFCGridItem)
                        {
                            // on supprime le hilite
                            MyElement_MouseLeave(ctrl, null);

                            idxRow = (int)ctrl.GetValue(Grid.RowProperty);
                            idxRow++;
                            ctrl.SetValue(Grid.RowProperty, idxRow);
                        }
                        else if (ctrl is JFCGridCellGrouping)
                        {
                            idxRow = (int)ctrl.GetValue(Grid.RowProperty);
                            idxRow++;
                            ctrl.SetValue(Grid.RowProperty, idxRow);

                            //int nbRowAffi = Parent.NbRowAffichable();
                            int indexData = Parent.ScrollVerticalValue + idxRow;

                            JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

                            JFCExpendItem datacontext = cg.DataContext as JFCExpendItem;

                            if (datacontext != null)
                            {
                                UpdateCellGroupingSizeAndVisibility(cg, indexData, null, datacontext);
                            }
                        }
                        else
                        {
                            idxRow = (int)ctrl.GetValue(Grid.RowProperty);
                            idxRow++;
                            ctrl.SetValue(Grid.RowProperty, idxRow);
                        }
                    }
                }
            }

            // on bouge la ligne
            foreach (var ctrl in TabLine[IndexAfter])
            {
                if (ctrl is Line)
                {
                    Line b = ctrl as Line;
                    if (b.Orientation == Orientation.Horizontal)
                    {
                        ctrl.SetValue(Grid.RowProperty, IndexAfter);
                    }
                }
                else if (ctrl is JFCGridItem)
                {
                    // on supprime le hilite
                    MyElement_MouseLeave(ctrl, null);

                    ctrl.SetValue(Grid.RowProperty, IndexAfter);
                }
                else if (ctrl is JFCGridCellGrouping)
                {
                    ctrl.SetValue(Grid.RowProperty, IndexAfter);
                }
                else
                {
                    ctrl.SetValue(Grid.RowProperty, IndexAfter);
                }
            }

        }

        public void RowMove(int IndexBefore, int IndexAfter)
        {
            //int idxRow;

            if (IndexBefore < IndexAfter)
            {
                if (TabLineH.Count() <= IndexAfter)
                    return;

                if (TabLineB.Count() <= IndexAfter)
                    return;

                if (TabLineF.Count() <= IndexAfter)
                    return;
            }

            if (IndexBefore > IndexAfter)
            {
                if (TabLineH.Count() <= IndexBefore)
                    return;

                if (TabLineB.Count() <= IndexBefore)
                    return;

                if (TabLineF.Count() <= IndexBefore)
                    return;
            }

            var ltmph = TabLineH[IndexBefore];
            TabLineH.RemoveAt(IndexBefore);
            TabLineH.Insert(IndexAfter, ltmph);

            var ltmpb = TabLineB[IndexBefore];
            TabLineB.RemoveAt(IndexBefore);
            TabLineB.Insert(IndexAfter, ltmpb);

            var ltmpf = TabLineF[IndexBefore];
            TabLineF.RemoveAt(IndexBefore);
            TabLineF.Insert(IndexAfter, ltmpf);

            RowMoveTab(TabLineH, IndexBefore, IndexAfter);
            RowMoveTab(TabLineB, IndexBefore, IndexAfter);
            RowMoveTab(TabLineF, IndexBefore, IndexAfter);


            /////////////////////////////////////
            //// Grid10

            //// on bouge les autres lignes
            //if (IndexBefore < IndexAfter)
            //{
            //    for (int i = IndexBefore; i < IndexAfter; i++)
            //    {
            //        foreach (var ctrl in TabLineH[i])
            //        {
            //            if (ctrl is Line)
            //            {
            //                Line b = ctrl as Line;
            //                if (b.Orientation == Orientation.Horizontal)
            //                {
            //                    idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                    idxRow--;
            //                    ctrl.SetValue(Grid.RowProperty, idxRow);
            //                }
            //            }
            //            else if (ctrl is JFCGridItem)
            //            {
            //                // on supprime le hilite
            //                MyElement_MouseLeave(ctrl, null);

            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow--;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);
            //            }
            //            else if (ctrl is JFCGridCellGrouping)
            //            {
            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow--;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);

            //                int nbRowAffi = Parent.NbRowAffichable();
            //                int indexData = Parent.ScrollVerticalValue + idxRow;

            //                JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

            //                JFCExpendItem datacontext = cg.DataContext as JFCExpendItem;

            //                if (datacontext != null)
            //                {
            //                    UpdateCellGroupingSizeAndVisibility(cg, indexData, null, datacontext);
            //                }
            //            }
            //            else
            //            {
            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow--;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);
            //            }
            //        }
            //    }
            //}
            //else if (IndexBefore > IndexAfter)
            //{
            //    for (int i = IndexBefore; i > IndexAfter; i--)
            //    {
            //        foreach (var ctrl in TabLineH[i])
            //        {
            //            if (ctrl is Line)
            //            {
            //                Line b = ctrl as Line;
            //                if (b.Orientation == Orientation.Horizontal)
            //                {
            //                    idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                    idxRow++;
            //                    ctrl.SetValue(Grid.RowProperty, idxRow);
            //                }
            //            }
            //            else if (ctrl is JFCGridItem)
            //            {
            //                // on supprime le hilite
            //                MyElement_MouseLeave(ctrl, null);

            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow++;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);
            //            }
            //            else if (ctrl is JFCGridCellGrouping)
            //            {
            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow++;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);

            //                int nbRowAffi = Parent.NbRowAffichable();
            //                int indexData = Parent.ScrollVerticalValue + idxRow;

            //                JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

            //                JFCExpendItem datacontext = cg.DataContext as JFCExpendItem;

            //                if (datacontext != null)
            //                {
            //                    UpdateCellGroupingSizeAndVisibility(cg, indexData, null, datacontext);
            //                }
            //            }
            //            else
            //            {
            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow++;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);
            //            }
            //        }
            //    }
            //}

            //// on bouge la ligne
            //foreach (var ctrl in TabLineH[IndexAfter])
            //{
            //    if (ctrl is Line)
            //    {
            //        Line b = ctrl as Line;
            //        if (b.Orientation == Orientation.Horizontal)
            //        {
            //            ctrl.SetValue(Grid.RowProperty, IndexAfter);
            //        }
            //    }
            //    else if (ctrl is JFCGridItem)
            //    {
            //        // on supprime le hilite
            //        MyElement_MouseLeave(ctrl, null);

            //        ctrl.SetValue(Grid.RowProperty, IndexAfter);
            //    }
            //    else if (ctrl is JFCGridCellGrouping)
            //    {
            //        ctrl.SetValue(Grid.RowProperty, IndexAfter);

            //        //int nbRowAffi = Parent.NbRowAffichable();
            //        //int indexData = Parent.ScrollVerticalValue + IndexAfter;

            //        //JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

            //        //JFCExpendItem datacontext = cg.DataContext as JFCExpendItem;

            //        //if (datacontext != null)
            //        //{
            //        //    UpdateCellGroupingSizeAndVisibility(cg, indexData, datacontext);
            //        //}
            //    }
            //    else
            //    {
            //        ctrl.SetValue(Grid.RowProperty, IndexAfter);
            //    }
            //}


            /////////////////////////////////////
            //// Grid11

            //// on bouge les autres lignes
            //if (IndexBefore < IndexAfter)
            //{
            //    for (int i = IndexBefore; i < IndexAfter; i++)
            //    {
            //        foreach (var ctrl in TabLineB[i])
            //        {
            //            if (ctrl is Line)
            //            {
            //                Line b = ctrl as Line;
            //                if (b.Orientation == Orientation.Horizontal)
            //                {
            //                    idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                    idxRow--;
            //                    ctrl.SetValue(Grid.RowProperty, idxRow);
            //                }
            //            }
            //            else if (ctrl is JFCGridItem)
            //            {
            //                // on supprime le hilite
            //                MyElement_MouseLeave(ctrl, null);

            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow--;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);
            //            }
            //            else if (ctrl is JFCGridCellGrouping)
            //            {
            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow--;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);

            //                int nbRowAffi = Parent.NbRowAffichable();
            //                int indexData = Parent.ScrollVerticalValue + idxRow;

            //                JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

            //                JFCExpendItem datacontext = cg.DataContext as JFCExpendItem;

            //                if (datacontext != null)
            //                {
            //                    UpdateCellGroupingSizeAndVisibility(cg, indexData, null, datacontext);
            //                }
            //            }
            //            else
            //            {
            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow--;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);
            //            }
            //        }
            //    }
            //}
            //else if (IndexBefore > IndexAfter)
            //{
            //    for (int i = IndexBefore; i > IndexAfter; i--)
            //    {
            //        foreach (var ctrl in TabLineB[i])
            //        {
            //            if (ctrl is Line)
            //            {
            //                Line b = ctrl as Line;
            //                if (b.Orientation == Orientation.Horizontal)
            //                {
            //                    idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                    idxRow++;
            //                    ctrl.SetValue(Grid.RowProperty, idxRow);
            //                }
            //            }
            //            else if (ctrl is JFCGridItem)
            //            {
            //                // on supprime le hilite
            //                MyElement_MouseLeave(ctrl, null);

            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow++;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);
            //            }
            //            else if (ctrl is JFCGridCellGrouping)
            //            {
            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow++;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);

            //                int nbRowAffi = Parent.NbRowAffichable();
            //                int indexData = Parent.ScrollVerticalValue + idxRow;

            //                JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

            //                JFCExpendItem datacontext = cg.DataContext as JFCExpendItem;

            //                if (datacontext != null)
            //                {
            //                    UpdateCellGroupingSizeAndVisibility(cg, indexData, null, datacontext);
            //                }
            //            }
            //            else
            //            {
            //                idxRow = (int)ctrl.GetValue(Grid.RowProperty);
            //                idxRow++;
            //                ctrl.SetValue(Grid.RowProperty, idxRow);
            //            }
            //        }
            //    }
            //}

            //// on bouge la ligne
            //foreach (var ctrl in TabLineB[IndexAfter])
            //{
            //    if (ctrl is Line)
            //    {
            //        Line b = ctrl as Line;
            //        if (b.Orientation == Orientation.Horizontal)
            //        {
            //            ctrl.SetValue(Grid.RowProperty, IndexAfter);
            //        }
            //    }
            //    else if (ctrl is JFCGridItem)
            //    {
            //        // on supprime le hilite
            //        MyElement_MouseLeave(ctrl, null);

            //        ctrl.SetValue(Grid.RowProperty, IndexAfter);
            //    }
            //    else if (ctrl is JFCGridCellGrouping)
            //    {
            //        ctrl.SetValue(Grid.RowProperty, IndexAfter);

            //        //int nbRowAffi = Parent.NbRowAffichable();
            //        //int indexData = Parent.ScrollVerticalValue + IndexAfter;

            //        //JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

            //        //JFCExpendItem datacontext = cg.DataContext as JFCExpendItem;

            //        //if (datacontext != null)
            //        //{
            //        //    UpdateCellGroupingSizeAndVisibility(cg, indexData, datacontext);
            //        //}
            //    }
            //    else
            //    {
            //        ctrl.SetValue(Grid.RowProperty, IndexAfter);
            //    }
            //}
        }

        public void RowMoveBegin(int Index)
        {
            RowMove(Index, 0);
        }

        public void RowMoveEnd(int Index)
        {
            int IndexAfter = Grid11.RowDefinitions.Count() - 1;
            RowMove(Index, IndexAfter);
        }

        public void RowMoveBeginToEnd()
        {
            int IndexAfter = Grid11.RowDefinitions.Count() - 1;
            RowRefreshData(IndexAfter, -1);
            RowMove(0, IndexAfter);
            RowRefreshData(IndexAfter);
        }

        public void RowMoveBeginToEnd(int NbRow)
        {
            int IndexAfter = Grid11.RowDefinitions.Count() - 1;

            for (int i = NbRow - 1; i >= 0; i--)
            {
                //RowRefreshData(0, -1);

                RowMove(0, IndexAfter);

                RowRefreshData(IndexAfter, Parent.ScrollVerticalValue + IndexAfter - i);
            }
        }

        public void RowMoveEndToBegin()
        {
            int Index = Grid11.RowDefinitions.Count() - 1;
            RowRefreshData(Index, -1);
            RowMove(Index, 0);
            RowRefreshData(0);
        }

        public void RowMoveEndToBegin(int NbRow)
        {
            int Index = Grid11.RowDefinitions.Count() - 1;
            //Object DataContext;

            for (int i = NbRow - 1; i >= 0; i--)
            {
                RowRefreshData(Index, -1);

                RowMove(Index, 0);

                //DataContext = Parent.dataSource[Parent.ScrollVerticalValue + i];
                RowRefreshData(0, Parent.ScrollVerticalValue + i);
            }
        }

        private void RowRemoveTab(List<LinkedList<UIElement>> TabLine, int Index, Grid gridtmp, int NbRow)
        {
            int idxr;

            if (TabLine.Count() <= Index)
                return;

            foreach (var ctrl in TabLine[Index])
            {
                if (ctrl is Line)
                {
                    Line b = ctrl as Line;
                    if (b.Orientation == Orientation.Horizontal)
                    {
                        gridtmp.Children.Remove(ctrl);
                        DeleteEvents(ctrl);
                    }
                }
                else
                {
                    gridtmp.Children.Remove(ctrl);
                    DeleteEvents(ctrl);

                    if (ctrl is CellImg)
                        LstRecycleCellImg.Add((CellImg)ctrl);
                    else if (ctrl is JFCGridCell)
                        LstRecycleJFCGridCell.Add((JFCGridCell)ctrl);
                }
            }

            gridtmp.RowDefinitions.RemoveAt(Index);

            for (int idxR = Index + 1; idxR < TabLine.Count(); idxR++)
            {
                foreach (var ctrl in TabLine[idxR])
                {
                    //idxr = (int)ctrl.GetValue(Grid.RowProperty);
                    //idxr--
                    idxr = idxR - 1;
                    ctrl.SetValue(Grid.RowProperty, idxr);
                }
            }

            // on réduit la ligne de séparation des colonnes            
            IEnumerable<UIElement> ListCtrl = gridtmp.Children.Cast<UIElement>();
            var ctrls = from ctrl in ListCtrl
                        where ctrl is Line
                        select ctrl;

            //List<UIElement> lctrlsBody1 = ctrlsBody1.ToList();

            foreach (var ctrl in ctrls)
            {
                Line l = ctrl as Line;
                if (l.Orientation == Orientation.Vertical)
                    ctrl.SetValue(Grid.RowSpanProperty, NbRow);
            }
        }

        public void RowRemove(int Index)
        {
            int idxr;
            int NbRow = Grid11.RowDefinitions.Count();



            ///////////////////////////////////
            // Grid10

            RowRemoveTab(TabLineH, Index, Grid10, NbRow);

            ///////////////////////////////////
            // Grid11

            RowRemoveTab(TabLineB, Index, Grid11, NbRow);

            ///////////////////////////////////
            // Grid12

            RowRemoveTab(TabLineF, Index, Grid12, NbRow);

            TabLineH.RemoveAt(Index);
            TabLineB.RemoveAt(Index);
            TabLineF.RemoveAt(Index);
        }

        public void RowFooterRemove(int Index)
        {
            int idxr;
            int NbRow = Grid11.RowDefinitions.Count();


            ///////////////////////////////////
            // Grid11

            foreach (var ctrl in TabLineBF[Index])
            {
                if (ctrl is Line)
                {
                    Line b = ctrl as Line;
                    if (b.Orientation == Orientation.Horizontal)
                    {
                        Grid21.Children.Remove(ctrl);
                        DeleteEvents(ctrl);
                    }
                }
                else
                {
                    Grid21.Children.Remove(ctrl);
                    DeleteEvents(ctrl);

                    if (ctrl is CellImg)
                        LstRecycleCellImg.Add((CellImg)ctrl);
                    else if (ctrl is JFCGridCell)
                        LstRecycleJFCGridCell.Add((JFCGridCell)ctrl);
                }
            }

            Grid21.RowDefinitions.RemoveAt(Index);

            for (int idxR = Index + 1; idxR < TabLineBF.Count(); idxR++)
            {
                foreach (var ctrl in TabLineBF[idxR])
                {
                    idxr = idxR - 1;
                    ctrl.SetValue(Grid.RowProperty, idxr);
                }
            }

            // on réduit la ligne de séparation des colonnes            
            IEnumerable<UIElement> ListCtrlBodyFooter1 = Grid21.Children.Cast<UIElement>();
            var ctrlsBody1 = from ctrl in ListCtrlBodyFooter1
                             where ctrl is Line
                             select ctrl;

            foreach (var ctrl in ctrlsBody1)
            {
                Line l = ctrl as Line;
                if (l.Orientation == Orientation.Vertical)
                    ctrl.SetValue(Grid.RowSpanProperty, NbRow);
            }

            //TabLineH.RemoveAt(Index);
            TabLineBF.RemoveAt(Index);
        }

        public void RowRemoveAll()
        {
            TabLineH.Clear();
            TabLineB.Clear();
            TabLineF.Clear();

            //Grid10.Children.Clear();
            for (int idx = 0; idx < Grid10.Children.Count; idx++)
            {
                UIElement ctrl = Grid10.Children[idx];
                if (ctrl is Line)
                {
                    if (((Line)ctrl).Orientation == Orientation.Horizontal)
                    {
                        Grid10.Children.Remove(ctrl);
                        idx--;
                        DeleteEvents(ctrl);
                    }
                }
                else
                {
                    Grid10.Children.Remove(ctrl);
                    idx--;
                    DeleteEvents(ctrl);

                    if (ctrl is CellImg)
                        LstRecycleCellImg.Add((CellImg)ctrl);
                    else if (ctrl is JFCGridCell)
                        LstRecycleJFCGridCell.Add((JFCGridCell)ctrl);
                }
            }

            //Grid11.Children.Clear();            
            for (int idx = 0; idx < Grid11.Children.Count; idx++)
            {
                UIElement ctrl = Grid11.Children[idx];
                if (ctrl is Line)
                {
                    if (((Line)ctrl).Orientation == Orientation.Horizontal)
                    {
                        Grid11.Children.Remove(ctrl);
                        idx--;
                        DeleteEvents(ctrl);
                    }
                }
                else
                {
                    Grid11.Children.Remove(ctrl);
                    idx--;
                    DeleteEvents(ctrl);

                    if (ctrl is CellImg)
                        LstRecycleCellImg.Add((CellImg)ctrl);
                    else if (ctrl is JFCGridCell)
                        LstRecycleJFCGridCell.Add((JFCGridCell)ctrl);
                }
            }

            for (int idx = 0; idx < Grid12.Children.Count; idx++)
            {
                UIElement ctrl = Grid12.Children[idx];
                if (ctrl is Line)
                {
                    if (((Line)ctrl).Orientation == Orientation.Horizontal)
                    {
                        Grid12.Children.Remove(ctrl);
                        idx--;
                        DeleteEvents(ctrl);
                    }
                }
                else
                {
                    Grid12.Children.Remove(ctrl);
                    idx--;
                    DeleteEvents(ctrl);

                    if (ctrl is CellImg)
                        LstRecycleCellImg.Add((CellImg)ctrl);
                    else if (ctrl is JFCGridCell)
                        LstRecycleJFCGridCell.Add((JFCGridCell)ctrl);
                }
            }

            Grid10.RowDefinitions.Clear();
            Grid11.RowDefinitions.Clear();
            Grid12.RowDefinitions.Clear();
        }

        public void RowFooterRemoveAll()
        {
            TabLineHF.Clear();
            TabLineBF.Clear();

            //Grid20.Children.Clear();
            for (int idx = 0; idx < Grid20.Children.Count; idx++)
            {
                UIElement ctrl = Grid20.Children[idx];
                if (ctrl is Line)
                {
                    if (((Line)ctrl).Orientation == Orientation.Horizontal)
                    {
                        Grid20.Children.Remove(ctrl);
                        idx--;
                        DeleteEvents(ctrl);
                    }
                }
                else
                {
                    Grid20.Children.Remove(ctrl);
                    idx--;
                    DeleteEvents(ctrl);

                    if (ctrl is CellImg)
                        LstRecycleCellImg.Add((CellImg)ctrl);
                    else if (ctrl is JFCGridCell)
                        LstRecycleJFCGridCell.Add((JFCGridCell)ctrl);
                }
            }

            //Grid21.Children.Clear();            
            for (int idx = 0; idx < Grid21.Children.Count; idx++)
            {
                UIElement ctrl = Grid21.Children[idx];
                if (ctrl is Line)
                {
                    if (((Line)ctrl).Orientation == Orientation.Horizontal)
                    {
                        Grid21.Children.Remove(ctrl);
                        idx--;
                        DeleteEvents(ctrl);
                    }
                }
                else
                {
                    Grid21.Children.Remove(ctrl);
                    idx--;
                    DeleteEvents(ctrl);

                    if (ctrl is CellImg)
                        LstRecycleCellImg.Add((CellImg)ctrl);
                    else if (ctrl is JFCGridCell)
                        LstRecycleJFCGridCell.Add((JFCGridCell)ctrl);
                }
            }

            Grid20.RowDefinitions.Clear();
            Grid21.RowDefinitions.Clear();
        }

        public void RowRefreshDataAll()
        {
            if (Parent?.dataSource != null)
            {
                int dataSourceCount = Parent.dataSource.Count();

                for (int Index = 0; Index < Grid11.RowDefinitions.Count(); Index++)
                {
                    if ((Parent.ScrollVerticalValue + Index) >= dataSourceCount)
                    {
                        RowRemove(Index);
                        Index--;
                    }
                    else
                    {
                        RowRefreshData(Index);
                    }
                }
            }
            else
            {
                RowRemoveAll();
            }
        }

        public void RowFooterRefreshDataAll()
        {
            if (Parent?.dataSourceFooter != null)
            {
                int dataSourceCount = Parent.dataSourceFooter.Count();

                for (int Index = 0; Index < Grid21.RowDefinitions.Count(); Index++)
                {
                    if (Grid21.RowDefinitions.Count > dataSourceCount)
                    {
                        RowFooterRemove(Index);
                        Index--;
                    }
                    else
                    {
                        RowFooterRefreshData(Index);
                    }
                }
            }
            else
            {
                RowFooterRemoveAll();
            }
        }

        public void RowRefreshDataAllAsync()
        {
            int dataSourceCount = Parent.dataSource.Count();

            //for (int Index = 0; Index < Grid11.RowDefinitions.Count(); Index++)
            int Index;
            for (Index = 0; Index < 10; Index++)
            {
                if ((Parent.ScrollVerticalValue + Index) >= dataSourceCount)
                {
                    RowRemove(Index);
                    Index--;
                }
                else
                {
                    RowRefreshData(Index);
                }
            }

            timer.Interval = new TimeSpan(0, 0, 0, 0, 100);

            timer.Tick -= timer_Tick;

            timer.Tag = Index;

            timer.Tick += new EventHandler(timer_Tick);

            timer.IsEnabled = true;
        }

        void timer_Tick(object sender, EventArgs e)
        {
            DispatcherTimer timer = sender as DispatcherTimer;
            int ind = (int)timer.Tag;

            int dataSourceCount = Parent.dataSource.Count();

            for (int Index = ind; Index < Grid11.RowDefinitions.Count(); Index++)
            {
                if ((Parent.ScrollVerticalValue + Index) >= dataSourceCount)
                {
                    RowRemove(Index);
                    Index--;
                }
                else
                {
                    RowRefreshData(Index);
                }
            }

            timer.IsEnabled = false;
        }

        public void RowRefreshData(int Index)
        {
            if ((Parent.ScrollVerticalValue + Index) >= Parent.dataSource.Count())
            {
                RowRemove(Index);
                return;
            }

            //Object DataContext = Parent.dataSource[Parent.ScrollVerticalValue + Index];

            //RowRefreshData(Index, DataContext, Parent.ScrollVerticalValue + Index);
            RowRefreshData(Index, Parent.ScrollVerticalValue + Index);
        }

        public void RowRefreshData(int Index, int IndexData)
        {
            RowRefreshData(Index, IndexData, null);
        }

        public void RefreshData(int Index, int IndexData, JFCGridColumn Column, List<LinkedList<UIElement>> TabLine, JFCExpendItem DataContext, IList<JFCGridColumn> ListCol)
        {

            CellImg cell;
            JFCGridCell cellTemplate;
            JFCGridItem item;

            //List<UIElement> CellBeforeDeleted = new List<UIElement>();

            if (TabLine.Count > Index && Index >= 0)
            {
                int nbCol = ListCol.Count();
                //if (ListCol.Count() > 0)
                //{
                foreach (var ctrl in TabLine[Index])
                {
                    if (nbCol > 0)
                    {
                        JFCGridColumn Col = null;

                        if (Column != null)
                        {
                            int idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);

                            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
                            {
                                Col = ListCol[idxCol];
                            }
                            else if (Column.Frozen == JFCGridColumn.FrozenType.End)
                            {
                                Col = ListCol[idxCol];
                            }
                            else
                            {
                                if (StartIndexColView + idxCol < ListCol.Count())
                                    Col = ListCol[StartIndexColView + idxCol];
                                else
                                    return;
                            }

                            if (Column != Col)
                                continue;
                        }

                        if (ctrl is CellImg)
                        {
                            cell = ctrl as CellImg;

                            if (DataContext == null)
                                cell.Data = null;
                            else
                            {
                                cell.Data = DataContext.Obj;

                                if (Parent.IsSearchVisible)
                                {
                                    if (Parent.SearchResult.Count() > 0)
                                    {
                                        if (Col == null)
                                        {
                                            int idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);

                                            if (ListCol == ListColBody)
                                                Col = ListCol[StartIndexColView + idxCol];
                                            else
                                                Col = ListCol[idxCol];
                                        }

                                        var lst = from s in Parent.SearchResult
                                                  where s.Column == Col && s.Item == DataContext
                                                  select s;

                                        if (lst.Count() >= 1)
                                            cell.HiliteText = Parent.TextBoxSearch.Text;
                                        else
                                            cell.HiliteText = "";
                                    }
                                    else
                                    {
                                        cell.HiliteText = "";
                                    }
                                }
                                else
                                    cell.HiliteText = "";
                            }
                        }
                        else if (ctrl is JFCGridCell)
                        {
                            cellTemplate = ctrl as JFCGridCell;

                            if (Col == null)
                            {
                                int idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);

                                if (ListCol == ListColBody)
                                    Col = ListCol[StartIndexColView + idxCol];
                                else
                                    Col = ListCol[idxCol];
                            }

                            object value;
                            if (DataContext != null && Col.GetCellValue(DataContext.Obj, out value))
                            {
                                cellTemplate.DataContext = value;
                            }
                            else if (Col.BindingPath != "" && Col.BindingPath != null && DataContext != null)
                            {
                                PropertyInfo data = DataContext.Obj.GetType().GetProperty(Col.BindingPath);

                                if (data != null)
                                {
                                    cellTemplate.DataContext = data.GetValue(DataContext.Obj, null);
                                }
                                else
                                {
                                    Binding binding = new Binding(Col.BindingPath);
                                    binding.Source = DataContext.Obj;
                                    cellTemplate.SetBinding(JFCGridCell.DataContextProperty, binding);
                                }
                            }
                            else
                            {
                                if (DataContext == null)
                                    cellTemplate.DataContext = null;
                                else
                                    cellTemplate.DataContext = DataContext.Obj;
                            }

                            if (Parent.IsSearchVisible)
                            {
                                if (Parent.SearchResult.Count() > 0)
                                {
                                    var lst = from s in Parent.SearchResult
                                              where s.Column == Col && s.Item == DataContext
                                              select s;

                                    if (lst.Count() >= 1)
                                        cellTemplate.HiliteText = Parent.TextBoxSearch.Text;
                                    else
                                        cellTemplate.HiliteText = "";
                                }
                                else
                                {
                                    cellTemplate.HiliteText = "";
                                }
                            }
                            else
                                cellTemplate.HiliteText = "";

                            foreach (var elm in TabLine[Index])
                            {
                                if (elm is JFCGridItem)
                                {
                                    if (((JFCGridItem)elm).Orientation == Orientation.Horizontal)
                                    {
                                        cellTemplate.ItemRow = (JFCGridItem)elm;
                                        break;
                                    }
                                }
                            }

                            cellTemplate.ItemColumn = Col.Item;

                        }
                        else if (ctrl is HierarchicalItem)
                        {
                            HierarchicalItem hi = ctrl as HierarchicalItem;

                            hi.DataContext = DataContext;

                            if (hi.Component is CellImg)
                            {
                                cell = hi.Component as CellImg;

                                if (DataContext == null)
                                    cell.Data = null;
                                else
                                {
                                    cell.Data = DataContext.Obj;

                                    if (Parent.IsSearchVisible)
                                    {
                                        if (Parent.SearchResult.Count() > 0)
                                        {
                                            if (Col == null)
                                            {
                                                int idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);

                                                if (ListCol == ListColBody)
                                                    Col = ListCol[StartIndexColView + idxCol];
                                                else
                                                    Col = ListCol[idxCol];
                                            }

                                            var lst = from s in Parent.SearchResult
                                                      where s.Column == Col && s.Item == DataContext
                                                      select s;

                                            if (lst.Count() >= 1)
                                                cell.HiliteText = Parent.TextBoxSearch.Text;
                                            else
                                                cell.HiliteText = "";
                                        }
                                        else
                                        {
                                            cell.HiliteText = "";
                                        }
                                    }
                                    else
                                        cell.HiliteText = "";
                                }
                            }
                            else
                            {
                                if (Col == null)
                                {
                                    int idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);

                                    if (ListCol == ListColBody)
                                        Col = ListCol[StartIndexColView + idxCol];
                                    else
                                        Col = ListCol[idxCol];
                                }

                                if (hi.Component is JFCGridCell)
                                {
                                    JFCGridCell c = hi.Component as JFCGridCell;

                                    if (Parent.IsSearchVisible)
                                    {
                                        if (Parent.SearchResult.Count() > 0)
                                        {
                                            var lst = from s in Parent.SearchResult
                                                      where s.Column == Col && s.Item == DataContext
                                                      select s;

                                            if (lst.Count() >= 1)
                                                c.HiliteText = Parent.TextBoxSearch.Text;
                                            else
                                                c.HiliteText = "";
                                        }
                                        else
                                        {
                                            c.HiliteText = "";
                                        }
                                    }
                                    else
                                        c.HiliteText = "";
                                }

                                object value;
                                if (DataContext != null && Col.GetCellValue(DataContext.Obj, out value))
                                {
                                    hi.Component.DataContext = value;
                                }
                                else if (Col.BindingPath != "" && Col.BindingPath != null && DataContext != null)
                                {
                                    PropertyInfo data = DataContext.Obj.GetType().GetProperty(Col.BindingPath);

                                    if (data != null)
                                    {
                                        hi.Component.DataContext = data.GetValue(DataContext.Obj, null);
                                    }
                                    else
                                    {
                                        Binding binding = new Binding(Col.BindingPath);
                                        binding.Source = DataContext.Obj;
                                        hi.Component.SetBinding(JFCGridCell.DataContextProperty, binding);
                                    }
                                }
                                else
                                {
                                    if (DataContext == null)
                                        hi.Component.DataContext = null;
                                    else
                                        hi.Component.DataContext = DataContext.Obj;
                                }
                            }
                        }
                        else if (ctrl is JFCGridCellGrouping)
                        {
                            JFCGridCellGrouping cg = ctrl as JFCGridCellGrouping;

                            int IdxRow = Index;
                            int IdxCol = (int)ctrl.GetValue(Grid.ColumnProperty);

                            if (Col == null)
                            {
                                if (ListCol == ListColBody)
                                    Col = ListCol[StartIndexColView + IdxCol];
                                else
                                    Col = ListCol[IdxCol];

                            }

                            JFCExpendItem dataContextTmp = DataContext;

                            if (Col.LevelGrouping >= 0)
                            {
                                while (dataContextTmp != null)
                                {
                                    if (dataContextTmp.Level == Col.LevelGrouping)
                                    {
                                        break;
                                    }

                                    dataContextTmp = dataContextTmp.Parent;
                                }
                            }
                            else
                            {
                                dataContextTmp = null;
                            }

                            cg.DataContext = dataContextTmp;

                            if (dataContextTmp != null)
                            {
                                if (Parent.SelectedExpendItems.Contains(dataContextTmp))
                                    cg.IsSelected = true;
                            }

                            cg.SetValue(Grid.ColumnProperty, IdxCol);
                            cg.SetValue(Grid.RowProperty, IdxRow);
                            cg.SetValue(Grid.ZIndexProperty, 1000);

                            UpdateCellGroupingSizeAndVisibility(cg, IndexData, Column, dataContextTmp);
                            //}


                            if (Parent.SelectedExpendItems.Contains(dataContextTmp) || Parent.SelectedExpendItemsChildren.Contains(dataContextTmp))
                            {
                                if (cg.IsSelected == false)
                                {
                                    cg.IsSelected = true;
                                }
                            }
                            else
                            {
                                if (cg.IsSelected == true)
                                {
                                    cg.IsSelected = false;
                                }
                            }

                            if (cg.Component is CellImg)
                            {
                                cell = cg.Component as CellImg;

                                if (dataContextTmp == null)
                                    cell.Data = null;
                                else
                                {
                                    cell.Data = dataContextTmp.Obj;

                                    if (Parent.IsSearchVisible)
                                    {
                                        if (Parent.SearchResult.Count() > 0)
                                        {
                                            //if (Col == null)
                                            //{
                                            //    int idxCol = (int)ctrl.GetValue(Grid.ColumnProperty);

                                            //    if (ListCol == ListColHeaderRow)
                                            //        Col = ListCol[idxCol];
                                            //    else
                                            //        Col = ListCol[StartIndexColView + idxCol];
                                            //}

                                            var lst = from s in Parent.SearchResult
                                                      where s.Column == Col && s.Item == dataContextTmp
                                                      select s;

                                            if (lst.Count() >= 1)
                                                cell.HiliteText = Parent.TextBoxSearch.Text;
                                            else
                                                cell.HiliteText = "";
                                        }
                                        else
                                        {
                                            cell.HiliteText = "";
                                        }
                                    }
                                    else
                                        cell.HiliteText = "";
                                }
                            }
                            else
                            {


                                if (cg.Component is JFCGridCell)
                                {
                                    JFCGridCell c = cg.Component as JFCGridCell;

                                    if (Parent.IsSearchVisible)
                                    {
                                        if (Parent.SearchResult.Count() > 0)
                                        {
                                            var lst = from s in Parent.SearchResult
                                                      where s.Column == Col && s.Item == dataContextTmp
                                                      select s;

                                            if (lst.Count() >= 1)
                                                c.HiliteText = Parent.TextBoxSearch.Text;
                                            else
                                                c.HiliteText = "";
                                        }
                                        else
                                        {
                                            c.HiliteText = "";
                                        }
                                    }
                                    else
                                        c.HiliteText = "";
                                }

                                object value;
                                if (dataContextTmp != null && Col.GetCellValue(dataContextTmp.Obj, out value))
                                {
                                    cg.Component.DataContext = value;
                                }
                                else if (Col.BindingPath != "" && Col.BindingPath != null && dataContextTmp != null)
                                {
                                    PropertyInfo data = dataContextTmp.Obj.GetType().GetProperty(Col.BindingPath);

                                    if (data != null)
                                    {
                                        cg.Component.DataContext = data.GetValue(dataContextTmp.Obj, null);
                                    }
                                    else
                                    {
                                        Binding binding = new Binding(Col.BindingPath);
                                        binding.Source = dataContextTmp.Obj;
                                        cg.Component.SetBinding(JFCGridCell.DataContextProperty, binding);
                                    }
                                }
                                else
                                {
                                    if (dataContextTmp == null)
                                        cg.Component.DataContext = null;
                                    else
                                        cg.Component.DataContext = dataContextTmp.Obj;
                                }
                            }
                        }
                    }

                    if (ctrl is JFCGridItem)
                    {
                        item = ctrl as JFCGridItem;
                        //item.DataContext = DataContext;
                        item.Data = DataContext;
                        item.IndexData = IndexData;

                        if ((IndexData % 2) == 0)
                            item.SetValue(JFCGridItem.IsAlternateProperty, true);
                        else
                            item.SetValue(JFCGridItem.IsAlternateProperty, false);

                        // on met a jour la selection
                        if (Parent.SelectedExpendItems.Contains(DataContext) || Parent.SelectedExpendItemsChildren.Contains(item.Data))
                        {
                            if (item.IsSelected == false)
                            {
                                item.IsSelected = true;
                                //item.Peer.IsSelected = true;
                                item.SelectedPeer(true);
                            }
                        }
                        else
                        {
                            if (item.IsSelected == true)
                            {
                                item.IsSelected = false;
                                //item.Peer.IsSelected = false;
                                item.SelectedPeer(false);
                            }
                        }

                    }
                    else if (ctrl is Line)
                    {
                        Line line = ctrl as Line;

                        if (DataContext != null)
                            line.LineBrush = Parent.HorizontalBorderColor;
                    }
                }

                ////on supprime les cellules à supprimer
                //foreach (var ctrl in CellBeforeDeleted)
                //{
                //    TabLine[Index].Remove(ctrl);
                //}
            }
        }

        public void RowRefreshData(int Index, int IndexData, JFCGridColumn Column)
        {

            if (IndexData >= Parent.dataSource.Count())
            {
                RowRemove(Index);
                return;
            }

            JFCExpendItem DataContext;

            if (IndexData < 0)
                DataContext = null;
            else
            {
                //DataContext = Parent.dataSource[IndexData];
                DataContext = Parent.dataSource[IndexData];
            }

            RefreshData(Index, IndexData, Column, TabLineH, DataContext, ListColHeaderRow);


            ////////////////////////////////////
            // Grid11

            RefreshData(Index, IndexData, Column, TabLineB, DataContext, ListColBody);

            RefreshData(Index, IndexData, Column, TabLineF, DataContext, ListColHeaderRowEnd);

        }

        public void RowFooterRefreshData(int Index)
        {
            //if ((Parent.ScrollVerticalValue + Index) >= Parent.dataSourceFooter.Count())
            //{
            //    RowRemove(Index);
            //    return;
            //}

            RowFooterRefreshData(Index, Index);
        }

        public void RowFooterRefreshData(int Index, int IndexData)
        {
            RowFooterRefreshData(Index, IndexData, null);
        }

        public void RowFooterRefreshData(int Index, int IndexData, JFCGridColumn Column)
        {

            if (IndexData >= Parent.dataSourceFooter.Count())
            {
                RowFooterRemove(Index);
                return;
            }

            JFCExpendItem DataContext;

            if (IndexData < 0)
                DataContext = null;
            else
                DataContext = Parent.dataSourceFooter[IndexData];


            RefreshData(Index, IndexData, Column, TabLineHF, DataContext, ListColHeaderFooter);


            ////////////////////////////////////
            // Grid11

            RefreshData(Index, IndexData, Column, TabLineBF, DataContext, ListColBody);

        }

        public void RefreshContextMenuRow()
        {
            RefreshContextMenuRow(TabLineH);
            RefreshContextMenuRow(TabLineHF);

            RefreshContextMenuRow(TabLineB);
            RefreshContextMenuRow(TabLineBF);
        }

        public void RefreshContextMenuRow(List<LinkedList<UIElement>> Tab)
        {
            for (int i = 0; i < Tab.Count(); i++)
            {
                foreach (var ctrl in Tab[i])
                {
                    if (ctrl is JFCGridItem)
                    {
                        JFCGridItem item = ctrl as JFCGridItem;

                        item.ContextMenu = Parent.ContextmenuRow;
                    }
                    else if (ctrl is Line)
                    {
                        Line bdG = ctrl as Line;

                        if (bdG.Orientation == Orientation.Horizontal)
                            bdG.ContextMenu = Parent.ContextmenuRow;
                    }
                    else if (ctrl is HierarchicalItem)
                    {
                        HierarchicalItem hi = ctrl as HierarchicalItem;
                        hi.ContextMenu = Parent.ContextmenuRow;
                        hi.Component.ContextMenu = Parent.ContextmenuRow;
                    }
                    else if (ctrl is FrameworkElement)
                    {
                        FrameworkElement fe = ctrl as FrameworkElement;
                        fe.ContextMenu = Parent.ContextmenuRow;
                    }
                }
            }
        }

        public enum ApplyMode
        {
            View,
            CalculSize
        }

        private void ApplyStyleCell(CellImg Cell, JFCGridColumn Column)
        {
            ApplyStyleCell(Cell, Column, ApplyMode.View);
        }

        private void ApplyStyleCell(CellImg Cell, JFCGridColumn Column, ApplyMode Mode)
        {
            //if (Column.Style == null)
            //{
            //// BackColor
            //Binding bBackColor = new Binding();
            //bBackColor.Source = Column.CellStyle.BackColor;
            //bBackColor.Mode = BindingMode.OneWay;

            //Cell.SetBinding(JFCGridCell.BackgroundProperty, bBackColor);

            //// ForeColor
            //Binding bForeColor = new Binding();
            //bForeColor.Source = Column.CellStyle.ForeColor;
            //bForeColor.Mode = BindingMode.OneWay;

            //Cell.SetBinding(JFCGridCell.ForegroundProperty, bForeColor);

            //// Margin
            //Binding bMargin = new Binding();
            //bMargin.Source = Column.CellStyle.Margin;
            //bMargin.Mode = BindingMode.OneWay;

            //Cell.SetBinding(JFCGridCell.MarginProperty, bMargin);

            // TextAlignment
            //Binding bTextAlignment = new Binding();
            //bTextAlignment.Source = Column.CellStyle.TextAlignment;
            //bTextAlignment.Mode = BindingMode.OneWay;

            //Cell.SetBinding(JFCGridCell.TextAlignmentProperty, bTextAlignment);

            if (Mode == ApplyMode.View)
            {
                Cell.TextAlignment = Column.CellStyle.TextAlignment;

                Column.CellStyle.PropertyChanged += delegate (object sender, System.ComponentModel.PropertyChangedEventArgs e)
                    {
                        Cell.TextAlignment = Column.CellStyle.TextAlignment;
                    };
            }

            //// WrapMode
            //Binding bWrapMode = new Binding();
            //bWrapMode.Source = Column.CellStyle.WrapMode;
            //bWrapMode.Mode = BindingMode.OneWay;

            ////Cell.SetBinding(JFCGridCell.TextWrappingProperty, bWrapMode);

            //// TODO
            //Cell.VerticalAlignment = VerticalAlignment.Center;
            //}
            //else
            //{
            //    //Binding bStyle = new Binding();
            //    //bStyle.Source = Column.Style;
            //    //bStyle.Mode = BindingMode.OneWay;

            //    //Cell.SetBinding(JFCGridCell.StyleProperty, bStyle);

            //    Cell.Style = Column.Style;
            //}

            //Binding bMargin = new Binding();
            //bMargin.Source = Column.CellPadding;
            //bMargin.Mode = BindingMode.OneWay;

            //Cell.SetBinding(JFCGridCell.MarginProperty, bMargin);


            Cell.Margin = Column.CellPadding;

            if (Mode == ApplyMode.View)
            {
                //Column.PropertyChanged += delegate (object sender, PropertyChangedExtendedEventArgs<Object> e)
                Column.PropertyChanged += delegate (object sender, System.ComponentModel.PropertyChangedEventArgs e)
                {
                    if (e.PropertyName == "Margin")
                        Cell.Margin = Column.CellPadding;

                    //if (e.PropertyName == "Style")
                    //    Cell.Style = Column.Style;
                };
            }

        }

        public void RefreshHorizontalBorderColor()
        {

            ////////////////////////////////////
            // Grid10



            IEnumerable<UIElement> ListCtrl = Grid10.Children.Cast<UIElement>();
            var ctrls = from ctrl in ListCtrl
                        where ctrl is Line
                        select ctrl;

            List<UIElement> lctrls = ctrls.ToList();

            foreach (var ctrl in lctrls)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Horizontal)
                    item.LineBrush = Parent.HorizontalBorderColor;                
            }

            ////////////////////////////////////
            // Grid20

            IEnumerable<UIElement> ListCtrlFooter = Grid20.Children.Cast<UIElement>();
            var ctrlsFooter = from ctrl in ListCtrlFooter
                              where ctrl is Line
                              select ctrl;

            List<UIElement> lctrlsFooter = ctrlsFooter.ToList();

            foreach (var ctrl in lctrls)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Horizontal)
                    item.LineBrush = Parent.HorizontalBorderColor;
            }

            ////////////////////////////////////
            // Grid11

            IEnumerable<UIElement> ListCtrlBody = Grid11.Children.Cast<UIElement>();
            var ctrlsBody = from ctrl in ListCtrlBody
                            where ctrl is Line
                            select ctrl;

            List<UIElement> lctrlsBody = ctrlsBody.ToList();

            foreach (var ctrl in lctrlsBody)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Horizontal)
                    item.LineBrush = Parent.HorizontalBorderColor;
            }

            ////////////////////////////////////
            // Grid21

            IEnumerable<UIElement> ListCtrlBodyFooter = Grid21.Children.Cast<UIElement>();
            var ctrlsBodyFooter = from ctrl in ListCtrlBodyFooter
                                  where ctrl is Line
                                  select ctrl;

            List<UIElement> lctrlsBodyFooter = ctrlsBodyFooter.ToList();

            foreach (var ctrl in lctrlsBodyFooter)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Horizontal)
                    item.LineBrush = Parent.HorizontalBorderColor;
            }

            ////////////////////////////////////
            // Grid12

            IEnumerable<UIElement> ListCtrlEndRow = Grid12.Children.Cast<UIElement>();
            var ctrlsEndRow = from ctrl in ListCtrlEndRow
                              where ctrl is Line
                              select ctrl;

            List<UIElement> lctrlsEndRow = ctrlsEndRow.ToList();

            foreach (var ctrl in lctrlsEndRow)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Horizontal)
                    item.LineBrush = Parent.HorizontalBorderColor;
            }

        }

        public void RefreshVerticalBorderColor()
        {

            ////////////////////////////////////
            // Grid10

            IEnumerable<UIElement> ListCtrl = Grid10.Children.Cast<UIElement>();
            var ctrls = from ctrl in ListCtrl
                        where ctrl is Line
                        select ctrl;

            List<UIElement> lctrls = ctrls.ToList();

            foreach (var ctrl in lctrls)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Vertical)
                    item.LineBrush = Parent.VerticalBorderColor;
            }

            ////////////////////////////////////
            // Grid20

            IEnumerable<UIElement> ListCtrlFooter = Grid20.Children.Cast<UIElement>();
            var ctrlsFooter = from ctrl in ListCtrlFooter
                              where ctrl is Line
                              select ctrl;

            List<UIElement> lctrlsFooter = ctrlsFooter.ToList();

            foreach (var ctrl in lctrlsFooter)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Vertical)
                    item.LineBrush = Parent.VerticalBorderColor;
            }

            ////////////////////////////////////
            // Grid11

            IEnumerable<UIElement> ListCtrlBody = Grid11.Children.Cast<UIElement>();
            var ctrlsBody = from ctrl in ListCtrlBody
                            where ctrl is Line
                            select ctrl;

            List<UIElement> lctrlsBody = ctrlsBody.ToList();

            foreach (var ctrl in lctrlsBody)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Vertical)
                    item.LineBrush = Parent.VerticalBorderColor;
            }

            ////////////////////////////////////
            // Grid21

            IEnumerable<UIElement> ListCtrlBodyFooter = Grid21.Children.Cast<UIElement>();
            var ctrlsBodyFooter = from ctrl in ListCtrlBodyFooter
                                  where ctrl is Line
                                  select ctrl;

            List<UIElement> lctrlsBodyFooter = ctrlsBodyFooter.ToList();

            foreach (var ctrl in lctrlsBodyFooter)
            {
                var item = ctrl as Line;

                if (item.Orientation == Orientation.Vertical)
                    item.LineBrush = Parent.VerticalBorderColor;
            }
        }

        private void UpdateCellGroupingSizeAndVisibility(JFCGridCellGrouping cg, int indexData, JFCGridColumn column, JFCExpendItem datacontext)
        {
            if (datacontext != null)
            {
                int nbRowAffi = Parent.NbRowAffichable();

                int nbrow = 0;

                nbrow = datacontext.NbChildrenView - (indexData - datacontext.StartChildViewIndex);

                int indexMax = (int)Parent.ScrollBarVertical.Value + nbRowAffi - 1;

                if (nbrow + indexData - 1 > indexMax)
                {
                    if (indexMax - indexData + 1 > 0)
                        cg.SetValue(Grid.RowSpanProperty, indexMax - indexData + 1);
                    else
                        cg.SetValue(Grid.RowSpanProperty, 1);
                }
                else
                {
                    if (nbrow > 0)
                        cg.SetValue(Grid.RowSpanProperty, nbrow);
                    else
                        cg.SetValue(Grid.RowSpanProperty, 1);
                }

                if (datacontext.StartChildViewIndex == indexData || Parent.ScrollVerticalValue == indexData)
                {
                    cg.Visibility = Visibility.Visible;
                }
                else
                {
                    cg.Visibility = Visibility.Hidden;
                }
            }
            else
            {
                cg.Visibility = Visibility.Hidden;
            }
        }

        #endregion

        #region Selection manager

        private void SelectionSimple(JFCGridItem Item)
        {
            // on supprime toute la selection            
            Parent.SelectedExpendItems.RemoveWhere(row => true);

            // on ajoute Item à la liste
            //Parent.SelectedItems.Add(Item.DataContext);
            Parent.SelectedExpendItems.Add(Item.Data);

            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());

        }

        private void SelectionSimple(IEnumerable<JFCExpendItem> lstItem)
        {
            // on supprime toute la selection            
            Parent.SelectedExpendItems.RemoveWhere(row => true);

            // on ajoute Item à la liste
            foreach (var item in lstItem)
            {
                Parent.SelectedExpendItems.Add(item);
            }

            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());

        }

        private void SelectionMultiple(JFCGridItem Item)
        {
            //if (Parent.SelectedItems.Contains(Item.DataContext))
            if (Parent.SelectedExpendItems.Contains(Item.Data))
            {
                // on supprime Item de la liste
                //Parent.SelectedItems.Remove(Item.DataContext);
                Parent.SelectedExpendItems.Remove(Item.Data);
            }
            else
            {
                // on ajoute Item à la liste
                //Parent.SelectedItems.Add(Item.DataContext);
                Parent.SelectedExpendItems.Add(Item.Data);
            }

            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());

        }

        private void SelectionMultiple(IEnumerable<JFCExpendItem> lstItem)
        {
            foreach (var item in lstItem)
            {
                if (Parent.SelectedExpendItems.Contains(item))
                {
                    // on supprime Item de la liste
                    Parent.SelectedExpendItems.Remove(item);
                }
                else
                {
                    // on ajoute Item à la liste
                    Parent.SelectedExpendItems.Add(item);
                }
            }


            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());

        }

        private void SelectionExtendedControl(JFCGridItem Item)
        {
            //if (Parent.SelectedItems.Contains(Item.DataContext))
            if (Parent.SelectedExpendItems.Contains(Item.Data))
            {
                // on supprime Item à la liste
                //Parent.SelectedItems.Remove(Item.DataContext);
                Parent.SelectedExpendItems.Remove(Item.Data);
            }
            else
            {
                // on ajoute Item à la liste
                //Parent.SelectedItems.Add(Item.DataContext);
                Parent.SelectedExpendItems.Add(Item.Data);
            }

            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());

        }

        private void SelectionExtendedControl(IEnumerable<JFCExpendItem> lstItem)
        {
            foreach (var item in lstItem)
            {
                if (Parent.SelectedExpendItems.Contains(item))
                {
                    // on supprime Item à la liste
                    Parent.SelectedExpendItems.Remove(item);
                }
                else
                {
                    // on ajoute Item à la liste
                    Parent.SelectedExpendItems.Add(item);
                }
            }


            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());

        }

        private void SelectionExtendedShift(JFCGridItem Item)
        {
            if (Parent.SelectedExpendItems.Count == 0)
            {
                // on ajoute le premier élément                
                Parent.SelectedExpendItems.Add(((JFCExpendItem)Parent.dataSource[0]));
            }
            else
            {
                // on supprime les selections sauf le premier
                //Parent.SelectedExpendItems.RemoveRange(1, Parent.SelectedExpendItems.Count - 1);
                var item = Parent.SelectedExpendItems.ElementAt(0);
                Parent.SelectedExpendItems.Clear();
                Parent.SelectedExpendItems.Add(item);
            }

            int IndexDebut = 0;
            int IndexFin = 0;
            bool FlagDebut = false;
            bool FlagFin = false;

            // on boucle sur les éléments 
            for (int Idx = 0; Idx < Parent.dataSource.Count(); Idx++)
            {

                if (Parent.dataSource[Idx] == Parent.SelectedExpendItems.ElementAt(0))
                {
                    IndexDebut = Idx;
                    FlagDebut = true;
                }

                //if (Parent.dataSource[Idx] == Item.DataContext)
                if (Parent.dataSource[Idx] == Item.Data)
                {
                    IndexFin = Idx;
                    FlagFin = true;
                }

                if (FlagDebut == true && FlagFin == true)
                    break;
            }


            //
            if (IndexDebut <= IndexFin)
            {
                // on boucle pour ajouter à la selection
                for (int Idx = IndexDebut + 1; Idx <= IndexFin; Idx++)
                {
                    Parent.SelectedExpendItems.Add(Parent.dataSource[Idx]);
                }
            }
            else
            {
                // on boucle pour ajouter à la selection
                for (int Idx = IndexFin; Idx < IndexDebut; Idx++)
                {
                    Parent.SelectedExpendItems.Add(Parent.dataSource[Idx]);
                }
            }

            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());

        }

        private void SelectionExtendedShift(IEnumerable<JFCExpendItem> lstItem)
        {
            if (Parent.SelectedExpendItems.Count == 0)
            {
                // on ajoute le premier élément                
                Parent.SelectedExpendItems.Add(((JFCExpendItem)Parent.dataSource[0]));
            }
            else
            {
                // on supprime les selections sauf le premier
                var item = Parent.SelectedExpendItems.ElementAt(0);
                Parent.SelectedExpendItems.Clear();
                Parent.SelectedExpendItems.Add(item);
            }

            int IndexDebut = 0;
            int IndexFin = 0;
            bool FlagDebut = false;
            bool FlagFin = false;

            // on boucle sur les éléments 
            for (int Idx = 0; Idx < Parent.dataSource.Count(); Idx++)
            {

                if (Parent.dataSource[Idx] == Parent.SelectedExpendItems.ElementAt(0))
                {
                    IndexDebut = Idx;
                    FlagDebut = true;
                }

                if (Parent.dataSource[Idx] == lstItem.ElementAt(0))
                {
                    IndexFin = Idx;
                    FlagFin = true;
                }

                if (FlagDebut == true && FlagFin == true)
                    break;
            }

            //
            if (IndexDebut <= IndexFin)
            {
                // on boucle pour ajouter à la selection
                for (int Idx = IndexDebut + 1; Idx <= IndexFin; Idx++)
                {
                    Parent.SelectedExpendItems.Add(Parent.dataSource[Idx]);
                }

                foreach (var item in lstItem)
                {
                    Parent.SelectedExpendItems.Add(item);
                }
            }
            else
            {
                // on boucle pour ajouter à la selection
                for (int Idx = IndexFin; Idx < IndexDebut; Idx++)
                {
                    Parent.SelectedExpendItems.Add(Parent.dataSource[Idx]);
                }
            }

            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());

        }

        public void SelectAll()
        {
            // on supprime toute la selection            
            Parent.SelectedExpendItems.RemoveWhere(row => true);

            // on ajoute tous les item
            //Parent.SelectedExpendItems.AddRange(Parent.dataSource);

            foreach (var item in Parent.dataSource)
            {
                Parent.SelectedExpendItems.Add(item);
            }

            RefreshSelection();

            // déclenche l'évennement
            Parent.OnSelectedItemsChanged(Parent, new RoutedEventArgs());
        }

        public void RefreshSelection()
        {

            // on rallonge la ligne de séparation des colonnes
            IEnumerable<UIElement> ListCtrlBody = Grid11.Children.Cast<UIElement>();
            var ctrlsBody = from ctrl in ListCtrlBody
                            where ctrl is JFCGridItem
                            select ctrl;

            foreach (var ctrl in ctrlsBody)
            {
                JFCGridItem item = ctrl as JFCGridItem;

                if (Parent.SelectedExpendItems.Contains(item.Data) || Parent.SelectedExpendItemsChildren.Contains(item.Data))
                {
                    if (item.IsSelected == false)
                    {
                        item.IsSelected = true;
                        item.SelectedPeer(true);
                    }
                }
                else
                {
                    if (item.IsSelected == true)
                    {
                        item.IsSelected = false;
                        item.SelectedPeer(false);
                    }
                }
            }

            IEnumerable<UIElement> ListCtrl10 = Grid10.Children.Cast<UIElement>();
            var ctrls10 = from ctrl in ListCtrl10
                          where ctrl is JFCGridCellGrouping
                          select ctrl;

            IEnumerable<UIElement> ListCtrl11 = Grid11.Children.Cast<UIElement>();
            var ctrls11 = from ctrl in ListCtrl11
                          where ctrl is JFCGridCellGrouping
                          select ctrl;

            IEnumerable<UIElement> ListCtrl12 = Grid12.Children.Cast<UIElement>();
            var ctrls12 = from ctrl in ListCtrl12
                          where ctrl is JFCGridCellGrouping
                          select ctrl;

            var ctrls = ctrls10.Concat(ctrls11).Concat(ctrls12);

            foreach (var ctrl in ctrls)
            {
                JFCGridCellGrouping gCell = ctrl as JFCGridCellGrouping;
                JFCExpendItem item = gCell.DataContext as JFCExpendItem;

                if (item != null)
                {
                    if (Parent.SelectedExpendItems.Contains(item) || Parent.SelectedExpendItemsChildren.Contains(item))
                    {
                        if (gCell.IsSelected == false)
                        {
                            gCell.IsSelected = true;
                        }
                    }
                    else
                    {
                        if (gCell.IsSelected == true)
                        {
                            gCell.IsSelected = false;
                        }
                    }
                }
            }




        }

        private void item_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            JFCGridItem Item = sender as JFCGridItem;

            if (Parent.SelectionMode == SelectionMode.Single)
            {
                SelectionSimple(Item);

                RefreshSelection();
            }
            else if (Parent.SelectionMode == SelectionMode.Multiple)
            {
                SelectionMultiple(Item);

                RefreshSelection();
            }
            else if (Parent.SelectionMode == SelectionMode.Extended)
            {
                if (Keyboard.IsKeyDown(Key.RightShift) || Keyboard.IsKeyDown(Key.LeftShift))
                {
                    SelectionExtendedShift(Item);
                }
                else if (!(Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl)))
                {
                    //if (Parent.SelectedExpendItems.Count <= 1)

                    if (!Parent.SelectedExpendItems.Contains(Item.Data))
                        SelectionSimple(Item);
                }

                RefreshSelection();
            }
        }

        private void item_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            JFCGridItem Item = sender as JFCGridItem;

            if (Parent.SelectionMode == SelectionMode.Extended)
            {
                if (Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl))
                {
                    SelectionExtendedControl(Item);
                }
                else if (!(Keyboard.IsKeyDown(Key.RightShift) || Keyboard.IsKeyDown(Key.LeftShift)))
                {
                    SelectionSimple(Item);
                }

                RefreshSelection();
            }
        }

        void item_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            JFCGridItem Item = sender as JFCGridItem;

            // on teste si l'item est sélectionné
            if (!Parent.SelectedExpendItems.Contains(Item.Data))
            {
                SelectionSimple(Item);

                RefreshSelection();
            }
        }

        void item_IsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {
            JFCGridItem item = sender as JFCGridItem;

            //int index =  Parent.dataSource item.Data;

            JFCGrid.DependencyPropertyLineOverChangedEventArgs.GridPosition pos;

            if (item.Parent != null)
            {
                if (item.Parent == Grid10)
                    pos = JFCGrid.DependencyPropertyLineOverChangedEventArgs.GridPosition.HeaderRow;
                else if (item.Parent == Grid11)
                    pos = JFCGrid.DependencyPropertyLineOverChangedEventArgs.GridPosition.BodyRow;
                else if (item.Parent == Grid20)
                    pos = JFCGrid.DependencyPropertyLineOverChangedEventArgs.GridPosition.HeaderFooterRow;
                else if (item.Parent == Grid21)
                    pos = JFCGrid.DependencyPropertyLineOverChangedEventArgs.GridPosition.BodyFooterRow;
                else
                    pos = JFCGrid.DependencyPropertyLineOverChangedEventArgs.GridPosition.None;


                if (item.IsMouseOverLine)
                {
                    if (item.Data != null)
                        this.Parent.OnLineOverChanged(this, new JFCGrid.DependencyPropertyLineOverChangedEventArgs(JFCGridItem.DataContextProperty, null, item.Data.Obj, pos, item.IndexData));
                }
                else
                {
                    if (item.Data != null)
                        this.Parent.OnLineOverChanged(this, new JFCGrid.DependencyPropertyLineOverChangedEventArgs(JFCGridItem.DataContextProperty, item.Data.Obj, null, pos, item.IndexData));
                }
            }
        }

        #endregion

    }
}


using System.Collections.Specialized;
using System.Collections.Generic;

namespace JFCGridControl
{

    /// <summary>  
    /// Represents a dynamic data collection that provides notifications when items get added, removed, or when the whole list is refreshed.  
    /// </summary>  
    /// <typeparam name="T"></typeparam>  
    public class ObservableCollection<T> : System.Collections.ObjectModel.ObservableCollection<T>
    {

        /// <summary>  
        /// Adds the elements of the specified collection to the end of the ObservableCollection(Of T).  
        /// </summary>  
        public void AddRange(IEnumerable<T> collection)
        {
            int startingIndex = this.Count;

            List<T> lst = new List<T>();
            lst.AddRange(collection);

            foreach (var i in collection) Items.Add(i);
            NotifyCollectionChangedEventArgs notify = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add,lst);
            OnCollectionChanged(notify);

            //this.CheckReentrancy();
            ////
            //// We need the starting index later
            ////
            //int startingIndex = this.Count;

            ////
            //// Add the items directly to the inner collection
            ////
            //foreach (var data in collection)
            //{
            //    this.Items.Add(data);
            //}


            ////
            //// Now raise the changed events
            ////

            //this.OnPropertyChanged(new System.ComponentModel.PropertyChangedEventArgs("Count"));
            //this.OnPropertyChanged(new System.ComponentModel.PropertyChangedEventArgs("Item[]"));

            ////
            //// We have to change our input of new items into an IList since that is what the
            //// event args require.
            ////

            ////var changedItems = new List<T>(collection);
            ////this.OnCollectionChanged(changedItems, startingIndex);

            //this.OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add);

        }

        /// <summary>  
        /// Removes the first occurence of each item in the specified collection from ObservableCollection(Of T).  
        /// </summary>  
        public void RemoveRange(IEnumerable<T> collection)
        {

            List<T> lst = new List<T>();
            lst.AddRange(collection);

            foreach (var i in collection) Items.Remove(i);
            //OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove));

            NotifyCollectionChangedEventArgs notify = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, lst);
            OnCollectionChanged(notify);
        }

        /// <summary>  
        /// Clears the current collection and replaces it with the specified item.  
        /// </summary>  
        public void Replace(T item)
        {
            ReplaceRange(new T[] { item });
        }
        /// <summary>  
        /// Clears the current collection and replaces it with the specified collection.  
        /// </summary>  
        public void ReplaceRange(IEnumerable<T> collection)
        {
            List<T> old = new List<T>(Items);
            Items.Clear();
            foreach (var i in collection) Items.Add(i);
            OnCollectionChanged(new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        /// <summary>  
        /// Initializes a new instance of the System.Collections.ObjectModel.ObservableCollection(Of T) class.  
        /// </summary>  
        public ObservableCollection()
            : base() { }

        /// <summary>  
        /// Initializes a new instance of the System.Collections.ObjectModel.ObservableCollection(Of T) class that contains elements copied from the specified collection.  
        /// </summary>  
        /// <param name="collection">collection: The collection from which the elements are copied.</param>  
        /// <exception cref="System.ArgumentNullException">The collection parameter cannot be null.</exception>  
        public ObservableCollection(IEnumerable<T> collection)
            : base(collection) { }


        public ObservableCollection(object Parent)
            : base() 
        {
            this.Parent = Parent;
        }

        public ObservableCollection(object Parent, IEnumerable<T> collection)
            : base(collection) 
        {
            this.Parent = Parent;
        }

        private object parent = null;
        public object Parent
        {
            get { return parent; }
            set
            {
                parent = value;

                OnPropertyChanged(new System.ComponentModel.PropertyChangedEventArgs("Parent"));
            }
        }
    }
}

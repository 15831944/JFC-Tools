using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Globalization;

namespace TarifsPresse_Codipress
{
	/// <summary>
	/// This class implement a fuzzy text matching algorithm based on Levenshtein distance.
	/// </summary>
	public class Levenshtein
	{
		public static string WORD_SPLIT = " \t.,;:!?'";
		public static string LINE_SPLIT = "/";
		//public static string REMOVE_AFTER = " - ";

		/// <summary>
		/// Constructor with default normalization handler
		/// </summary>
		public Levenshtein()
			: this(Normalize)
		{ }

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="normalizeHandler">Text normalization handler</param>
		public Levenshtein(Func<string, string> normalizeHandler)
		{
			WordSplit = WORD_SPLIT.ToCharArray();
			LineSplit = LINE_SPLIT.ToCharArray();
			RemoveAfter = null;
			LineSubPrefix = null;
			NormalizeHandler = normalizeHandler;

			IsCancelHandler = () => false;
		}

		/// <summary>
		/// Normalize handler that is used on the sentences/words
		/// </summary>
		public Func<string, string> NormalizeHandler { get; private set; }

		/// <summary>
		/// Word separators
		/// </summary>
		public char[] WordSplit { get; set; }

		public Func<bool> IsCancelHandler { get; set; }

		/// <summary>
		/// Line separator: used to make a search on several sentences.
		/// For example: search1 OR search2 OR search3....
		/// </summary>
		public char[] LineSplit { get; set; }

		/// <summary>
		/// Line subtract prefix: used to make a search on several sentences but the one prefixed with the LineSubPrefix.
		/// For example: search1 OR search2 BUT NOT search3
		/// </summary>
		public string LineSubPrefix { get; set; }

		/// <summary>
		/// Remove from the search text all chars after the RemoveAfter pattern
		/// </summary>
		public string RemoveAfter { get; set; }

		/// <summary>
		/// Text table where to find the search sentence
		/// </summary>
		public Sentence[] Text { get; private set; }

		/// <summary>
		/// Setup the text table
		/// </summary>
		/// <param name="text"></param>
		public void SetupTextSentences(string[] text)
		{
			List<Sentence> list = new List<Sentence>(text.Length);

			foreach (var item in text)
			{
				Sentence sentence = new Sentence(item.Trim());

				sentence.Setup(NormalizeHandler, WordSplit);

				list.Add(sentence);
			}

			Text = list.ToArray();
		}

		/// <summary>
		/// Search a sentence
		/// </summary>
		/// <param name="search">the sentence to search</param>
		/// <returns>The best match of senetence</returns>
		public IEnumerable<KeyValuePair<double, List<int>>> SearchSentenceRank(string search)
		{
			double min_score;
			double max_score;

			return SearchSentenceRank(search, out min_score, out max_score);
		}

		/// <summary>
		/// Search a Word
		/// </summary>
		/// <param name="search">the word to search</param>
		/// <returns>The best match of sentences that contains the word</returns>
		public IEnumerable<KeyValuePair<double, List<int>>> SearchWordRank(string search)
		{
			double min_score;
			double max_score;

			return SearchWordRank(search, out min_score, out max_score);
		}

		/// <summary>
		/// Search a sentence
		/// </summary>
		/// <param name="search"></param>
		/// <param name="min_score"></param>
		/// <param name="max_score"></param>
		/// <returns></returns>
		public IEnumerable<KeyValuePair<double, List<int>>> SearchSentenceRank(string search, out double min_score, out double max_score)
		{
			return SearchRank(search, out min_score, out max_score, SearchSentenceRank_Parallel);
		}

		/// <summary>
		/// Search a Word
		/// </summary>
		/// <param name="search"></param>
		/// <param name="min_score"></param>
		/// <param name="max_score"></param>
		/// <returns></returns>
		public IEnumerable<KeyValuePair<double, List<int>>> SearchWordRank(string search, out double min_score, out double max_score)
		{
			return SearchRank(search, out min_score, out max_score, SearchWordRank_Parallel);
		}


		/// <summary>
		/// Compute the score between two sentences
		/// </summary>
		/// <param name="word1"></param>
		/// <param name="word2"></param>
		/// <returns></returns>
		public double ScoreSentences(string s1, string s2)
		{
			Sentence sentence1 = new Sentence(s1.Trim());
			Sentence sentence2 = new Sentence(s2.Trim());

			sentence1.Setup(NormalizeHandler, WordSplit);
			sentence2.Setup(NormalizeHandler, WordSplit);

			return ComputeScoreSentence(ref sentence1, ref sentence2);
		}

		/// <summary>
		/// Compute the score between two words
		/// </summary>
		/// <param name="word1"></param>
		/// <param name="word2"></param>
		/// <returns>1.0 if the two words are equal and less than 1.0 otherwise</returns>
		public double ScoreWords(string word1, string word2)
		{
			return Base.CalculateLevenshteinScore(NormalizeHandler(word1), NormalizeHandler(word2));
		}

		/// <summary>
		/// A sentence description
		/// </summary>
		public struct Sentence
		{
			public Sentence(string text)
			{
				Text = text;
				NormalizedText = null;
				Words = null;
			}

			public string Text;

			public string NormalizedText;
			public string[] Words;

			public void Setup(Func<string, string> normalizeHandler, char[] wordSplit)
			{
				string normTxt = normalizeHandler(Text);
				NormalizedText = normTxt;
				Words = normTxt.Split(wordSplit).Where(t => string.Empty != t).ToArray();
			}

			public override string ToString()
			{
				return Text;
			}
		}

		#region Private methods...

		private IEnumerable<KeyValuePair<double, List<int>>> SearchRank(string search, out double min_score, out double max_score, SearchDelegate searchHandler)
		{
			Sentence[] text = Text;

			if (RemoveAfter != null && RemoveAfter != string.Empty)
			{
				int skipIndex = search.IndexOf(RemoveAfter);
				if (skipIndex != -1)
					search = search.Substring(0, skipIndex);
			}

			Dictionary<double, List<int>> matching = new Dictionary<double, List<int>>();
			// compute scoring.
			double[] scoring = new double[text.Length];
			for (int i = 0; i < text.Length; i++)
			{
				scoring[i] = double.MinValue;
			}

			min_score = double.MaxValue;
			max_score = double.MinValue;


			if (LineSplit == null)
			{
				Sentence searchItem = new Sentence(search.Trim());
				searchItem.Setup(NormalizeHandler, WordSplit);

				searchHandler(ref searchItem, text, matching, scoring, ref min_score, ref max_score);
			}
			else
			{
				List<string> addition = new List<string>();
				List<string> subtract = new List<string>();

				if (LineSubPrefix == null || LineSubPrefix == string.Empty)
				{
					addition.AddRange(search.Split(LineSplit));
				}
				else
				{
					var lines = search.Split(LineSplit);

					foreach (var item in lines)
					{
						if (item.StartsWith(LineSubPrefix))
							subtract.Add(item.Substring(LineSubPrefix.Length));
						else
							addition.Add(item);
					}
				}

				ScoreLines(addition, text, scoring, out min_score, out max_score, searchHandler, matching);

				if (subtract.Any())
				{
					Dictionary<double, List<int>> submatching = new Dictionary<double, List<int>>();
					// compute scoring.
					double[] subscoring = new double[text.Length];
					for (int i = 0; i < text.Length; i++)
					{
						subscoring[i] = double.MinValue;
					}
					double sub_min_score = double.MaxValue;
					double sub_max_score = double.MinValue;
					ScoreLines(subtract, text, subscoring, out sub_min_score, out sub_max_score, searchHandler, submatching);

					for (int i = 0; i < text.Length; i++)
					{
						double oldScore = scoring[i];
						double subScore = subscoring[i];
						if (subScore >= 0.3 && oldScore > 0)
						{

							//double factor = 1 - subScore;    1 - (1 - p)0.5
							// subScore in [0.5, 1] but we need subScore in [0,1]
							double factor = Math.Pow(1 - ((subScore - 0.3) * (1 / 0.7)), 0.20);

							matching[oldScore].Remove(i);
							if (matching[oldScore].Count == 0)
								matching.Remove(oldScore);

							double score = oldScore * factor;
							scoring[i] = score;

							List<int> list;
							if (!matching.TryGetValue(score, out list))
							{
								list = new List<int>();
								matching.Add(score, list);

								if (score < min_score)
									min_score = score;

								if (score > max_score)
									max_score = score;
							}
							list.Add(i);
						}
					}
				}
			}
			return (from p in matching
					orderby p.Key descending
					select p).ToArray();
		}


		private void ScoreLines(List<string> addition, Sentence[] text, double[] scoring, out double min_score, out double max_score, SearchDelegate searchHandler, Dictionary<double, List<int>> matching)
		{
			min_score = double.MaxValue;
			max_score = double.MinValue;

			foreach (var searchline in addition)
			{
				Dictionary<double, List<int>> wmatching = new Dictionary<double, List<int>>();
				// compute scoring.
				double[] wscoring = new double[text.Length];
				for (int i = 0; i < text.Length; i++)
				{
					wscoring[i] = double.MinValue;
				}

				Sentence searchItem = new Sentence(searchline.Trim());
				searchItem.Setup(NormalizeHandler, WordSplit);

				searchHandler(ref searchItem, text, wmatching, wscoring, ref min_score, ref max_score);

				for (int i = 0; i < text.Length; i++)
				{
					var oldScore = scoring[i];
					var score = wscoring[i];
					if (score > oldScore)
					{
						if (oldScore != double.MinValue)
						{
							matching[oldScore].Remove(i);
							if (matching[oldScore].Count == 0)
								matching.Remove(oldScore);
						}
						scoring[i] = score;

						List<int> list;
						if (!matching.TryGetValue(score, out list))
						{
							list = new List<int>();
							matching.Add(score, list);

							if (score < min_score)
								min_score = score;

							if (score > max_score)
								max_score = score;
						}
						list.Add(i);
					}
				}
			}
		}

		private delegate void SearchDelegate(ref Sentence search, Sentence[] text,
			Dictionary<double, List<int>> matching, double[] scoring, ref double min_score, ref double max_score);

		private void SearchSentenceRank(ref Sentence search, Sentence[] text,
			Dictionary<double, List<int>> matching, double[] scoring, ref double min_score, ref double max_score)
		{
			int index = 0;
			foreach (var textitem in text)
			{
				if (IsCancelHandler())
				{
					break;
				}

				Sentence item = textitem;

				double score = double.MinValue;
				if (item.NormalizedText == search.NormalizedText)
				{
					score = 1.0;
				}
				else
				{
					score = ComputeScoreSentence(ref search, ref item);
				}

				if (score > scoring[index])
				{
					var oldScore = scoring[index];
					if (oldScore != double.MinValue)
					{
						matching[oldScore].Remove(index);
						if (matching[oldScore].Count == 0)
							matching.Remove(oldScore);
					}
					scoring[index] = score;

					List<int> list;
					if (!matching.TryGetValue(score, out list))
					{
						list = new List<int>();
						matching.Add(score, list);

						if (score < min_score)
							min_score = score;

						if (score > max_score)
							max_score = score;
					}
					list.Add(index);
				}

				index++;
			}
		}

		private void SearchWordRank(ref Sentence search, Sentence[] text,
			Dictionary<double, List<int>> matching, double[] scoring, ref double min_score, ref double max_score)
		{
			int index = 0;
			foreach (var textitem in text)
			{
				if (IsCancelHandler())
				{
					break;
				}

				Sentence item = textitem;

				double score = double.MinValue;

				if (item.NormalizedText == search.NormalizedText)
				{
					score = 1.0;
				}
				else if (item.NormalizedText.Contains(search.NormalizedText))
				{
					bool done = false;
					// see if this is one full word
					foreach (var word in item.Words)
					{
						if (word == search.NormalizedText)
						{
							score = 1;
							done = true;
							break;
						}
						else if (word.StartsWith(search.NormalizedText))
						{
							double ratio = ((double)search.NormalizedText.Length / (double)word.Length);
							score = (ComputeScoreWord(ref search, ref item) + ratio) / 2;
							done = true;
							break;
						}
						else if (word.Contains(search.NormalizedText))
						{
							double ratio = ((double)search.NormalizedText.Length / (double)word.Length);
							score = (ComputeScoreWord(ref search, ref item) + (ratio / 2)) / 2;
							done = true;
							break;
						}
					}

					if (!done)
					{
						if (search.Words.Length > 1)
						{
							StringBuilder ns_search = new StringBuilder();
							for (int w = 0; w < search.Words.Length; w++)
							{
								ns_search.Append(search.Words[w]);
							}
							string str_ns_search = ns_search.ToString();

							StringBuilder buf = new StringBuilder();
							for (int n = 0; n <= item.Words.Length - search.Words.Length; n++)
							{
								buf.Clear();

								for (int w = 0; w < search.Words.Length; w++)
								{
									buf.Append(item.Words[n + w]);
								}

								if (str_ns_search.Equals(buf.ToString()))
								{
									score = 1;
									done = true;
									break;
								}
							}
						}
					}

					if (!done)
					{
						score = 0.70;
					}
				}
				else
				{
					score = ComputeScoreWord(ref search, ref item);
				}

				if (score > scoring[index])
				{
					var oldScore = scoring[index];
					if (oldScore != double.MinValue)
					{
						matching[oldScore].Remove(index);
						if (matching[oldScore].Count == 0)
							matching.Remove(oldScore);
					}
					scoring[index] = score;

					List<int> list;
					if (!matching.TryGetValue(score, out list))
					{
						list = new List<int>();
						matching.Add(score, list);

						if (score < min_score)
							min_score = score;

						if (score > max_score)
							max_score = score;
					}
					list.Add(index);
				}

				index++;
			}
		}

		int PROCESSOR_COUNT = 0;

		private void SearchSentenceRank_Parallel(ref Sentence search, Sentence[] text,
			Dictionary<double, List<int>> matching, double[] scoring, ref double min_score, ref double max_score)
		{
			Sentence theSearch = search;
			double the_min_score = min_score;
			double the_max_score = max_score;

			PROCESSOR_COUNT = Environment.ProcessorCount;
			if (PROCESSOR_COUNT > 1)
			{
				PROCESSOR_COUNT--;
			}

			double[] minarray = new double[PROCESSOR_COUNT];
			double[] maxarray = new double[PROCESSOR_COUNT];

			for (int i = 0; i < PROCESSOR_COUNT; i++)
			{
				minarray[i] = min_score;
				maxarray[i] = max_score;
			}

			if (PROCESSOR_COUNT == 1)
			{
				int p = 0;
				var newMatching = SearchSentenceRank_Parallel(p, ref theSearch, text, matching, scoring, ref minarray[p], ref maxarray[p]);
				foreach (var item in newMatching)
				{
					matching.Add(item.Key, item.Value);
				}
			}
			else
			{
				var parallelTasks = Enumerable.Range(0, PROCESSOR_COUNT)
					.Select(p => Task.Factory.StartNew(() => SearchSentenceRank_Parallel(p, ref theSearch, text, matching, scoring, ref minarray[p], ref maxarray[p])
						, TaskCreationOptions.AttachedToParent | TaskCreationOptions.PreferFairness)).ToArray();

				Task.WaitAll();

				foreach (var item in parallelTasks.Select(p => p.Result))
				{
					foreach (var item2 in item)
					{
						if (!matching.ContainsKey(item2.Key))
							matching.Add(item2.Key, item2.Value);
						else
						{
							var temp = matching[item2.Key];
							var res2 = item2.Value.Where(p => !temp.Contains(p));

							if (res2.Count() > 0)
								matching[item2.Key].AddRange(res2);
						}
					}
				}
			}
			min_score = minarray.Min();
			max_score = maxarray.Max();
		}

		private Dictionary<double, List<int>> SearchSentenceRank_Parallel(int startFrom, ref Sentence search, Sentence[] text, Dictionary<double, List<int>> matching,
			double[] scoring, ref double min_score, ref double max_score)
		{
			int index = startFrom;
			Dictionary<double, List<int>> result = new Dictionary<double, List<int>>();

			unsafe
			{
				// First we need to allocate a buffer for the Matrix
				int maxTextLen = 0;
				for (int i = startFrom; i < text.Length; i += PROCESSOR_COUNT)
				{
					int len = text[i].NormalizedText.Length;
					if (len > maxTextLen)
						maxTextLen = len;
				}
				int searchTextLen = search.NormalizedText.Length;

				int bufferSize = (maxTextLen + 1) * (searchTextLen + 1) * sizeof(float);

				fixed (byte* bufferPtr = new byte[bufferSize])
				{

					for (int i = startFrom; i < text.Length; i += PROCESSOR_COUNT)
					{
						index = i;
						Sentence item = text[i];
						if (IsCancelHandler())
						{
							break;
						}

						double score = double.MinValue;
						if (item.NormalizedText == search.NormalizedText)
						{
							score = 1.0;
						}
						else
						{
							score = ComputeScoreSentence_Unsafe(ref search, ref item, bufferPtr);

							//score = ComputeScoreSentence(ref search, ref item);
						}

						if (score > scoring[index])
						{
							var oldScore = scoring[index];
							if (oldScore != double.MinValue)
							{
								result[oldScore].Remove(index);
								if (result[oldScore].Count == 0)
									result.Remove(oldScore);
							}
							scoring[index] = score;

							List<int> list;
							if (!result.TryGetValue(score, out list))
							{
								list = new List<int>();
								result.Add(score, list);

								if (score < min_score)
									min_score = score;

								if (score > max_score)
									max_score = score;
							}
							list.Add(index);
						}
					}
				}
			}
			return result;
		}

		private void SearchWordRank_Parallel(ref Sentence search, Sentence[] text,
			Dictionary<double, List<int>> matching, double[] scoring, ref double min_score, ref double max_score)
		{
			Sentence theSearch = search;
			double the_min_score = min_score;
			double the_max_score = max_score;

			PROCESSOR_COUNT = Environment.ProcessorCount;
			if (PROCESSOR_COUNT > 1)
			{
				PROCESSOR_COUNT--;
			}

			double[] minarray = new double[PROCESSOR_COUNT];
			double[] maxarray = new double[PROCESSOR_COUNT];

			for (int i = 0; i < PROCESSOR_COUNT; i++)
			{
				minarray[i] = min_score;
				maxarray[i] = max_score;
			}

			if (PROCESSOR_COUNT == 1)
			{
				int p = 0;
				var newMatching = SearchWordRank_Parallel(p, ref theSearch, text, matching, scoring, ref minarray[p], ref maxarray[p]);
				foreach (var item in newMatching)
				{
					matching.Add(item.Key, item.Value);
				}
			}
			else
			{

				var parallelTasks = Enumerable.Range(0, PROCESSOR_COUNT)
				   .Select(p => Task.Factory.StartNew(() => SearchWordRank_Parallel(p, ref theSearch, text, matching, scoring, ref minarray[p], ref maxarray[p])
					   , TaskCreationOptions.AttachedToParent | TaskCreationOptions.PreferFairness)).ToArray();

				Task.WaitAll();

				foreach (var item in parallelTasks.Select(p => p.Result))
				{
					foreach (var item2 in item)
					{
						if (!matching.ContainsKey(item2.Key))
							matching.Add(item2.Key, item2.Value);
						else
						{
							var temp = matching[item2.Key];
							var res2 = item2.Value.Where(p => !temp.Contains(p));

							if (res2.Count() > 0)
								matching[item2.Key].AddRange(res2);
						}
					}
				}
			}

			min_score = minarray.Min();
			max_score = maxarray.Max();

		}

		private Dictionary<double, List<int>> SearchWordRank_Parallel(int startFrom, ref Sentence search, Sentence[] text, Dictionary<double, List<int>> matching,
			double[] scoring, ref double min_score, ref double max_score)
		{
			int index = startFrom;
			Dictionary<double, List<int>> result = new Dictionary<double, List<int>>();

			unsafe
			{
				// First we need to allocate a buffer for the Matrix
				int maxTextLen = 0;
				for (int i = startFrom; i < text.Length; i += PROCESSOR_COUNT)
				{
					int len = text[i].NormalizedText.Length;
					if (len > maxTextLen)
						maxTextLen = len;
				}
				int searchTextLen = search.NormalizedText.Length;

				int bufferSize = (maxTextLen + 1) * (searchTextLen + 1) * sizeof(float);

				fixed (byte* bufferPtr = new byte[bufferSize])
				{
					for (int i = startFrom; i < text.Length; i += PROCESSOR_COUNT)
					{
						index = i;
						Sentence item = text[i];
						if (IsCancelHandler())
						{
							break;
						}

						double score = double.MinValue;

						if (item.NormalizedText == search.NormalizedText)
						{
							score = 1.0;
						}
						else if (item.NormalizedText.Contains(search.NormalizedText))
						{
							bool done = false;
							// see if this is one full word
							foreach (var word in item.Words)
							{
								if (word == search.NormalizedText)
								{
									score = 1;
									done = true;
									break;
								}
								else if (word.StartsWith(search.NormalizedText))
								{
									double ratio = ((double)search.NormalizedText.Length / (double)word.Length);
									score = (ComputeScoreWord_Unsafe(ref search, ref item, bufferPtr) + ratio) / 2;
									done = true;
									break;
								}
								else if (word.Contains(search.NormalizedText))
								{
									double ratio = ((double)search.NormalizedText.Length / (double)word.Length);
									score = (ComputeScoreWord_Unsafe(ref search, ref item, bufferPtr) + (ratio / 2)) / 2;
									done = true;
									break;
								}
							}

							if (!done)
							{
								if (search.Words.Length > 1)
								{
									StringBuilder ns_search = new StringBuilder();
									for (int w = 0; w < search.Words.Length; w++)
									{
										ns_search.Append(search.Words[w]);
									}
									string str_ns_search = ns_search.ToString();

									StringBuilder buf = new StringBuilder();
									for (int n = 0; n <= item.Words.Length - search.Words.Length; n++)
									{
										buf.Clear();

										for (int w = 0; w < search.Words.Length; w++)
										{
											buf.Append(item.Words[n + w]);
										}

										if (str_ns_search.Equals(buf.ToString()))
										{
											score = 1;
											done = true;
											break;
										}
									}
								}
							}

							if (!done)
							{
								score = 0.70;
							}
						}
						else
						{
							score = ComputeScoreWord_Unsafe(ref search, ref item, bufferPtr);
						}

						if (score > scoring[index])
						{
							var oldScore = scoring[index];
							if (oldScore != double.MinValue)
							{
								result[oldScore].Remove(index);
								if (result[oldScore].Count == 0)
									result.Remove(oldScore);
							}
							scoring[index] = score;

							List<int> list;
							if (!result.TryGetValue(score, out list))
							{
								list = new List<int>();
								result.Add(score, list);

								if (score < min_score)
									min_score = score;

								if (score > max_score)
									max_score = score;
							}
							list.Add(index);
						}
					}
				}
			}

			return result;
		}

		private static unsafe double ComputeScoreSentence_Unsafe(ref Sentence search, ref Sentence item, byte* bufferPtr)
		{
			List<double> scoreList = new List<double>();
			int length = Math.Max(search.Words.Length, item.Words.Length);

			scoreList.Add(1.0 - Base.CalculateLevenshteinValue_Unsafe(search.Words, item.Words, (float*)bufferPtr) / length);
			//System.Console.Out.WriteLine(string.Format("Score = {0} ({1} vs {2})", score, search, item));

			scoreList.Add(Base.CalculateLevenshteinScore_Unsafe(search.NormalizedText, item.NormalizedText, (short*)bufferPtr));

			//scoreList.Add(1.0 - Base.CalculateLevenshteinValueExact(search.Words, item.Words) / length);

			StringBuilder nospace_search = new StringBuilder();
			foreach (var w in search.Words)
			{
				nospace_search.Append(w);
			}
			string ns_search = nospace_search.ToString();

			//foreach (var wi in item.Words)
			//{
			//    if (wi.Contains(ns_search))
			//    {
			//        scoreList.Add(1);
			//        //scoreList.Add(1);
			//        //scoreList.Add(1);
			//        //scoreList.Add(1);
			//    }
			//}

			StringBuilder nospace_item = new StringBuilder();
			foreach (var w in item.Words)
			{
				nospace_item.Append(w);
			}


			string ns_item = nospace_item.ToString();

			if (ns_item.Contains(ns_search))
			{
				//scoreList.Add(1);
				scoreList.Add(1);
			}
			else
			{
				scoreList.Add(Base.CalculateLevenshteinScore_Unsafe(ns_search, ns_item, (short*)bufferPtr));
			}
			return scoreList.Average();
		}



		private static double ComputeScoreSentence(ref Sentence search, ref Sentence item)
		{
			List<double> scoreList = new List<double>();
			int length = Math.Max(search.Words.Length, item.Words.Length);

			scoreList.Add(1.0 - Base.CalculateLevenshteinValue(search.Words, item.Words) / length);
			//System.Console.Out.WriteLine(string.Format("Score = {0} ({1} vs {2})", score, search, item));

			scoreList.Add(Base.CalculateLevenshteinScore(search.NormalizedText, item.NormalizedText));

			//scoreList.Add(1.0 - Base.CalculateLevenshteinValueExact(search.Words, item.Words) / length);

			StringBuilder nospace_search = new StringBuilder();
			foreach (var w in search.Words)
			{
				nospace_search.Append(w);
			}
			string ns_search = nospace_search.ToString();

			//foreach (var wi in item.Words)
			//{
			//    if (wi.Contains(ns_search))
			//    {
			//        scoreList.Add(1);
			//        //scoreList.Add(1);
			//        //scoreList.Add(1);
			//        //scoreList.Add(1);
			//    }
			//}

			StringBuilder nospace_item = new StringBuilder();
			foreach (var w in item.Words)
			{
				nospace_item.Append(w);
			}


			string ns_item = nospace_item.ToString();

			if (ns_item.Contains(ns_search))
			{
				//scoreList.Add(1);
				scoreList.Add(1);
			}
			else
			{
				scoreList.Add(Base.CalculateLevenshteinScore(ns_search, ns_item));
			}
			return scoreList.Average();
		}

		private static double ComputeScoreWord(ref Sentence search, ref Sentence item)
		{
			List<double> scoreList = new List<double>();
			int length = Math.Max(search.Words.Length, item.Words.Length);

			scoreList.Add(1.0 - Base.CalculateLevenshteinValue(search.Words, item.Words) / length);
			//System.Console.Out.WriteLine(string.Format("Score = {0} ({1} vs {2})", score, search, item));

			scoreList.Add(Base.CalculateLevenshteinScore(search.NormalizedText, item.NormalizedText));

			scoreList.Add(1.0 - Base.CalculateLevenshteinValueExact(search.Words, item.Words) / length);

			StringBuilder nospace_search = new StringBuilder();
			foreach (var w in search.Words)
			{
				nospace_search.Append(w);
			}
			string ns_search = nospace_search.ToString();

			foreach (var wi in item.Words)
			{
				if (wi.Contains(ns_search))
				{
					scoreList.Add(0.80);
					//scoreList.Add(1);
					//scoreList.Add(1);
					//scoreList.Add(1);
				}
			}

			StringBuilder nospace_item = new StringBuilder();
			foreach (var w in item.Words)
			{
				nospace_item.Append(w);
			}


			string ns_item = nospace_item.ToString();

			if (ns_item.Contains(ns_search))
			{
				scoreList.Add(0.75);
				//scoreList.Add(1);
			}
			else
			{
				scoreList.Add((Base.CalculateLevenshteinScore(ns_search, ns_item) + scoreList.Max()) / 2);
			}
			return scoreList.Max();
		}

		private static unsafe double ComputeScoreWord_Unsafe(ref Sentence search, ref Sentence item, byte* bufferPtr)
		{
			List<double> scoreList = new List<double>();
			int length = Math.Max(search.Words.Length, item.Words.Length);

			scoreList.Add(1.0 - Base.CalculateLevenshteinValue_Unsafe(search.Words, item.Words, (float*)bufferPtr) / length);
			//System.Console.Out.WriteLine(string.Format("Score = {0} ({1} vs {2})", score, search, item));

			scoreList.Add(Base.CalculateLevenshteinScore_Unsafe(search.NormalizedText, item.NormalizedText, (short*)bufferPtr));

			scoreList.Add(1.0 - Base.CalculateLevenshteinValueExact_Unsafe(search.Words, item.Words, (float*)bufferPtr) / length);

			StringBuilder nospace_search = new StringBuilder();
			foreach (var w in search.Words)
			{
				nospace_search.Append(w);
			}
			string ns_search = nospace_search.ToString();

			foreach (var wi in item.Words)
			{
				if (wi.Contains(ns_search))
				{
					scoreList.Add(0.80);
					//scoreList.Add(1);
					//scoreList.Add(1);
					//scoreList.Add(1);
				}
			}

			StringBuilder nospace_item = new StringBuilder();
			foreach (var w in item.Words)
			{
				nospace_item.Append(w);
			}


			string ns_item = nospace_item.ToString();

			if (ns_item.Contains(ns_search))
			{
				scoreList.Add(0.75);
				//scoreList.Add(1);
			}
			else
			{
				scoreList.Add((Base.CalculateLevenshteinScore_Unsafe(ns_search, ns_item, (short*)bufferPtr) + scoreList.Max()) / 2);
			}
			return scoreList.Max();
		}

		#endregion

		/// <summary>
		/// Base Levenshtein methods
		/// </summary>
		public static class Base
		{

			/// <summary>
			/// Compute Levenshtein distance
			/// </summary>
			/// <param name="source_word"></param>
			/// <param name="target_word"></param>
			/// <returns></returns>
			public static double CalculateLevenshteinScore(string source_word, string target_word)
			{
				int levenshtein = CalculateLevenshteinValue(source_word, target_word);

				int length = Math.Max(source_word.Length, target_word.Length);
				double score = 1.0 - (double)levenshtein / length;

				return score;
			}

			public static unsafe double CalculateLevenshteinScore_Unsafe(string source_word, string target_word, short* arrayPtr)
			{
				int levenshtein = CalculateLevenshteinValue_Unsafe(source_word, target_word, arrayPtr);

				int length = Math.Max(source_word.Length, target_word.Length);
				double score = 1.0 - (double)levenshtein / length;

				return score;
			}

			public static unsafe double CalculateLevenshteinScore_Unsafe_on_stack(string source_word, string target_word)
			{
				short* arrayPtr = stackalloc short[(source_word.Length + 1) * (target_word.Length + 1)];
				int levenshtein = CalculateLevenshteinValue_Unsafe(source_word, target_word, arrayPtr);

				int length = Math.Max(source_word.Length, target_word.Length);
				double score = 1.0 - (double)levenshtein / length;

				return score;
			}

			/// <summary>
			/// Compute the Levenshtein value between the two strings
			/// </summary>
			/// <param name="p"></param>
			/// <param name="p_2"></param>
			/// <returns></returns>
			public static unsafe int CalculateLevenshteinValue_Unsafe(string source, string target, short* arrayPtr)
			{
				var sourceLength = source.Length;
				int targetLength = target.Length;


				int lineLen = sourceLength + 1;
				int bufsize = lineLen * (targetLength + 1);

				//fixed (int* arrayPtr = new int[bufsize])
				{

					//int[] arrayPtr = new int[lineLen * (targetLength + 1)];
					// array[ src , tgt ] = arrayPtr[tgt * lineLen + src]
					//var array = new int[sourceLength + 1, targetLength + 1];


					short* arrayPtrEnd = arrayPtr + sourceLength;

					short index = 0;
					for (short* iptr = arrayPtr; iptr <= arrayPtrEnd; iptr++)
					{
						*iptr = index++;
					}

					//for (int i = 0; i <= sourceLength; i++)
					//{
					//    arrayPtr[i] = i;
					//}

					index = 0;
					arrayPtrEnd = arrayPtr + bufsize - 1;
					for (short* iptr = arrayPtr; iptr <= arrayPtrEnd; iptr += lineLen)
					{
						*iptr = index++;
					}

					//for (int i = 1; i <= targetLength; i++)
					//{
					//    arrayPtr[i * lineLen] = i;
					//}

					int lineLen_plus1 = lineLen + 1;

					for (int i = 1; i <= sourceLength; i++)
					{
						short* lineptr = arrayPtr + lineLen + i;

						for (int j = 1; j <= targetLength; j++, lineptr += lineLen)
						{

							// lineptr = array[i, j]

							var equal = (source[i - 1] == target[j - 1]) ? 0 : 2;
							//var minInsert = array[i - 1, j] + 1;
							//var minDelete = array[i, j - 1] + 1;

							//var minInsert = arrayPtr[j * lineLen + (i - 1)] + 1;
							//var minDelete = arrayPtr[(j - 1) * lineLen + i] + 1;
							var minInsert = *(lineptr - 1) + 1;
							var minDelete = *(lineptr - lineLen) + 1;

							//var minInterchange = array[i - 1, j - 1] + equal;
							//var minInterchange = arrayPtr[(j - 1) * lineLen + i - 1] + equal;
							//var minInterchange = *(lineptr - (lineLen + 1)) + equal;
							var minInterchange = *(lineptr - lineLen_plus1) + equal;
							//array[i, j] = Math.Min(Math.Min(minInsert, minDelete), minInterchange);
							//arrayPtr[j * lineLen + i] = Math.Min(Math.Min(minInsert, minDelete), minInterchange);
							*(lineptr) = (short)Math.Min(Math.Min(minInsert, minDelete), minInterchange);

						}
					}

					//return array[sourceLength, targetLength];
					//return arrayPtr[targetLength * lineLen + sourceLength];
					return *(arrayPtr + bufsize - 1);
				}
			}


			public static int CalculateLevenshteinValue(string source, string target)
			{
				var sourceLength = source.Length;
				var targetLength = target.Length;

				var array = new int[sourceLength + 1, targetLength + 1];
				for (int i = 0; i <= source.Length; i++)
				{
					array[i, 0] = i;
				}
				for (int i = 0; i <= targetLength; i++)
				{
					array[0, i] = i;
				}
				for (int i = 1; i <= sourceLength; i++)
				{
					for (int j = 1; j <= targetLength; j++)
					{
						var equal = (source[i - 1] == target[j - 1]) ? 0 : 2;
						var minInsert = array[i - 1, j] + 1;
						var minDelete = array[i, j - 1] + 1;
						var minInterchange = array[i - 1, j - 1] + equal;
						array[i, j] = Math.Min(Math.Min(minInsert, minDelete), minInterchange);
					}
				}

				return array[sourceLength, targetLength];
			}


			public static unsafe double CalculateLevenshteinValue_Unsafe(string[] source_words, string[] target_words, float* arrayPtr)
			{
				var sourceLength = source_words.Length;
				var targetLength = target_words.Length;

				int lineLen = source_words.Length + 1;
				int bufsize = lineLen * (targetLength + 1);
				//fixed (double* arrayPtr = new double[bufsize])
				{
					//double[] arrayPtr = new double[lineLen * (targetLength + 1)];
					//var array = new double[sourceLength + 1, targetLength + 1];

					float* arrayPtrEnd = arrayPtr + sourceLength;

					int index = 0;
					for (float* iptr = arrayPtr; iptr <= arrayPtrEnd; iptr++)
					{
						*iptr = index++;
					}

					index = 0;
					arrayPtrEnd = arrayPtr + bufsize - 1;
					for (float* iptr = arrayPtr; iptr <= arrayPtrEnd; iptr += lineLen)
					{
						*iptr = index++;
					}

					int lineLen_plus1 = lineLen + 1;

					for (int i = 1; i <= sourceLength; i++)
					{
						float* lineptr = arrayPtr + lineLen + i;

						for (int j = 1; j <= targetLength; j++, lineptr += lineLen)
						{
							//var equal = (CalculateLevenshteinDistance(source_words[i - 1], target_words[j - 1]) > 0.0) ? 0 : 2;
							var equal = 1.0 - Base.CalculateLevenshteinScore_Unsafe_on_stack(source_words[i - 1], target_words[j - 1]);

							//var minInsert = arrayPtr[j * lineLen + (i - 1)] + 1;
							//var minDelete = arrayPtr[(j - 1) * lineLen + i] + 1;
							//var minInterchange = arrayPtr[(j - 1) * lineLen + i - 1] + equal;
							//arrayPtr[j * lineLen + i] = Math.Min(Math.Min(minInsert, minDelete), minInterchange);
							var minInsert = *(lineptr - 1) + 1;
							var minDelete = *(lineptr - lineLen) + 1;
							var minInterchange = *(lineptr - lineLen_plus1) + equal;
							*(lineptr) = (float)Math.Min(Math.Min(minInsert, minDelete), minInterchange);
						}
					}
					//return arrayPtr[targetLength * lineLen + sourceLength];
					return *(arrayPtr + bufsize - 1);
				}
			}

			public static double CalculateLevenshteinValue(string[] source_words, string[] target_words)
			{
				var sourceLength = source_words.Length;
				var targetLength = target_words.Length;
				var array = new double[sourceLength + 1, targetLength + 1];
				for (int i = 0; i <= source_words.Length; i++)
				{
					array[i, 0] = i;
				}
				for (int i = 0; i <= target_words.Length; i++)
				{
					array[0, i] = i;
				}
				for (int i = 1; i <= source_words.Length; i++)
				{
					for (int j = 1; j <= target_words.Length; j++)
					{
						//var equal = (CalculateLevenshteinDistance(source_words[i - 1], target_words[j - 1]) > 0.0) ? 0 : 2;
						var equal = 1.0 - Base.CalculateLevenshteinScore(source_words[i - 1], target_words[j - 1]);
						var minInsert = array[i - 1, j] + 1;
						var minDelete = array[i, j - 1] + 1;
						var minInterchange = array[i - 1, j - 1] + equal;
						array[i, j] = Math.Min(Math.Min(minInsert, minDelete), minInterchange);
					}
				}

				return array[sourceLength, targetLength];
			}

			/// <summary>
			/// with exact word matching
			/// </summary>
			/// <param name="source"></param>
			/// <param name="source_words"></param>
			/// <param name="target"></param>
			/// <param name="target_words"></param>
			/// <returns></returns>
			public static double CalculateLevenshteinValueExact(string[] source_words, string[] target_words)
			{
				var sourceLength = source_words.Length;
				var targetLength = target_words.Length;
				var array = new double[sourceLength + 1, targetLength + 1];
				for (int i = 0; i <= source_words.Length; i++)
				{
					array[i, 0] = i;
				}
				for (int i = 0; i <= target_words.Length; i++)
				{
					array[0, i] = i;
				}
				for (int i = 1; i <= source_words.Length; i++)
				{
					for (int j = 1; j <= target_words.Length; j++)
					{
						//var equal = (CalculateLevenshteinDistance(source_words[i - 1], target_words[j - 1]) > 0.0) ? 0 : 2;
						//var equal = 1.0 - CalculateLevenshteinDistance(source_words[i - 1], target_words[j - 1]);
						var equal = (source_words[i - 1] == target_words[j - 1]) ? 0.0 : 2.0;
						var minInsert = array[i - 1, j] + 1;
						var minDelete = array[i, j - 1] + 1;
						var minInterchange = array[i - 1, j - 1] + equal;
						array[i, j] = Math.Min(Math.Min(minInsert, minDelete), minInterchange);
					}
				}

				return array[sourceLength, targetLength];
			}



			public static unsafe double CalculateLevenshteinValueExact_Unsafe(string[] source_words, string[] target_words, float* arrayPtr)
			{
				var sourceLength = source_words.Length;
				var targetLength = target_words.Length;

				int lineLen = source_words.Length + 1;
				int bufsize = lineLen * (targetLength + 1);
				//fixed (double* arrayPtr = new double[bufsize])
				{
					float* arrayPtrEnd = arrayPtr + sourceLength;

					int index = 0;
					for (float* iptr = arrayPtr; iptr <= arrayPtrEnd; iptr++)
					{
						*iptr = index++;
					}

					index = 0;
					arrayPtrEnd = arrayPtr + bufsize - 1;
					for (float* iptr = arrayPtr; iptr <= arrayPtrEnd; iptr += lineLen)
					{
						*iptr = index++;
					}

					int lineLen_plus1 = lineLen + 1;


					for (int i = 1; i <= source_words.Length; i++)
					{
						float* lineptr = arrayPtr + lineLen + i;
						for (int j = 1; j <= target_words.Length; j++, lineptr += lineLen)
						{
							//var equal = (CalculateLevenshteinDistance(source_words[i - 1], target_words[j - 1]) > 0.0) ? 0 : 2;
							//var equal = 1.0 - CalculateLevenshteinDistance(source_words[i - 1], target_words[j - 1]);
							var equal = (source_words[i - 1] == target_words[j - 1]) ? 0.0 : 2.0;
							//var minInsert = array[i - 1, j] + 1;
							//var minDelete = array[i, j - 1] + 1;
							//var minInterchange = array[i - 1, j - 1] + equal;
							//array[i, j] = Math.Min(Math.Min(minInsert, minDelete), minInterchange);
							var minInsert = *(lineptr - 1) + 1;
							var minDelete = *(lineptr - lineLen) + 1;
							var minInterchange = *(lineptr - lineLen_plus1) + equal;
							*(lineptr) = (float)Math.Min(Math.Min(minInsert, minDelete), minInterchange);
						}
					}

					//return array[sourceLength, targetLength];
					return *(arrayPtr + bufsize - 1);
				}
			}



		}

		#region Text normalization...

		private static Dictionary<char, string> NORMALIZE_MAP = new Dictionary<char, string>();
		static Levenshtein()
		{
			NORMALIZE_MAP.Add('=', " = ");
			NORMALIZE_MAP.Add('+', " + ");
			NORMALIZE_MAP.Add('-', " - ");
			NORMALIZE_MAP.Add('*', " * ");
			NORMALIZE_MAP.Add('/', " / ");
			NORMALIZE_MAP.Add('&', " & ");
			NORMALIZE_MAP.Add('@', " @ ");
			NORMALIZE_MAP.Add('(', " ");
			NORMALIZE_MAP.Add(')', " ");
			NORMALIZE_MAP.Add('[', " ");
			NORMALIZE_MAP.Add(']', " ");
			NORMALIZE_MAP.Add('{', " ");
			NORMALIZE_MAP.Add('}', " ");
		}

		public static string Normalize(string search)
		{
			StringBuilder buf = new StringBuilder();

			foreach (Char ch in search.ToLower().Normalize(NormalizationForm.FormKD))
			{
				var chCat = Char.GetUnicodeCategory(ch);
				if (chCat != UnicodeCategory.NonSpacingMark)
				{
					string sout;
					if (NORMALIZE_MAP.TryGetValue(ch, out sout))
						buf.Append(sout);
					else
						buf.Append(ch);
				}
			}

			return buf.ToString();
		}

		#endregion

	}

}

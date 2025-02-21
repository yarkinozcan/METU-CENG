:- module(main, [is_blogpost_written_by/2, has_post_in_topic/2, get_follower_count_of_blogger/2, get_post_count_of_topic/2, filter_posts_by_date/4, recommend_posts/2, recommend_bloggers/2]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE



is_blogpost_written_by(BloggerNick, ShortPostName) :-
    posted(BloggerNick, X), % Check if the blogger written a post with an ID same as the ID of the post with the name ShortPostName
    blogpost(X, ShortPostName, _, _). % Here Topic and and Date of the post is not important 



has_post_in_topic(BloggerNick, Topic) :-
    posted(BloggerNick, X), % Check if the blogger written a post with an ID same as the ID of the post with the topic Topic
    blogpost(X, _, Topic, _), % Here Name of the post and date is not important 
    !.  % There may be more than 1 post by the same blogger with the same topic, so in order to return 1 True use !



get_follower_count_of_blogger(BloggerNick, FollowerCount) :-
    findall(X, follows(X, BloggerNick), Y), % We Try find all nicknames of the followers of BloggerNick and putting to the list Y
    length(Y, FollowerCount). % We are finding the length of the list (number of followers) and then assigning that number to FollowerCount


get_post_count_of_topic(Topic, PostCount) :-
    findall(X, blogpost(X, _, Topic, _), Y), % We Try find all PostIDs of the posts with the topic Topic and putting to the list Y
    length(Y, PostCount). % We are finding the length of the list (number of Posts with the topic Topic) adn then assigning that number to PostCount


filter_posts_by_date(ListOfPostIDs, OldestDate, NewestDate, ListOfFilteredPostIDs) :-
    include(date_helper(OldestDate, NewestDate), ListOfPostIDs, ListOfFilteredPostIDs). % This built in predicate helps to filter a given list with the predicate function (in this case date_helper) 

date_helper(OldestDate, NewestDate, PostID) :- % Helper predicate to check whether the date of the post with the ID PostID is in the range 
    blogpost(PostID, _, _, Date), Date >= OldestDate, Date =< NewestDate.


recommend_posts(ReaderNick, ListOfRecommendedPosts) :-
    reader(ReaderNick, ListOfInterestTopics), findall(PostID, ( posted(BloggerNick, PostID), blogpost(PostID, _, Topic, _), member(Topic, ListOfInterestTopics), \+ alreadyread(ReaderNick, PostID)), ListOfRecommendedPosts).


% Predicate to recommend bloggers to a reader based on their interests and unfollowed bloggers
recommend_bloggers(ReaderNick, ListOfRecommendedBloggers) :-
    reader(ReaderNick, ListOfInterestTopics),
    findall(BloggerNick, (
        blogger(BloggerNick),
        \+ follows(ReaderNick, BloggerNick),
        posted(BloggerNick, PostID),
        blogpost(PostID, _, Topic, _),
        member(Topic, ListOfInterestTopics)
    ), Bloggers),
    sort(Bloggers, ListOfRecommendedBloggers).  % Remove duplicates and sort

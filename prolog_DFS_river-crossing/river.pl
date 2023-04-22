unsafe([1,0,0,_]). 
unsafe([0,1,1,_]).

unsafe([1,_,0,0]).
unsafe([0,_,1,1]).

% Sự kiện farmer đi riêng
cross([F,X,Y,Z], [F1,X,Y,Z]) :- 
    F1 is 1 - F,
    \+ unsafe([F1,X,Y,Z]).

% Sự kiện di chuyển qua sông (farmer đi chung với đậu)
cross([F,F,Y,Z], [F1,F1,Y,Z]) :- 
    F1 is 1 - F,
    \+ unsafe([F1,F1,Y,Z]).

% Sự kiện di chuyển qua sông (farmer đi chung với ngỗng)
cross([F,X,F,Z], [F1,X,F1,Z]) :- 
    F1 is 1 - F,
    \+ unsafe([F1,X,F1,Z]).

% Sự kiện di chuyển qua sông (farmer đi chung với cáo)
cross([F,X,Y,F], [F1,X,Y,F1]) :- 
    F1 is 1 - F,
    \+ unsafe([F1,X,Y,F1]).

% Kiểm tra xem trò chơi đã kết thúc hay chưa
end_state([1,1,1,1]).

% Tạo một danh sách trạng thái di chuyển hợp lệ
valid_moves(S, ValidMoves) :-
    % format("inside valid_moves S: "),print_list([S]),
    findall(NewState, cross(S, NewState), NextStates),
    filter_states(NextStates, ValidMoves).

% Lọc các trạng thái hợp lệ

print_path(A):-
    maplist(term_to_atom,A,RevHistoryAtoms),
    atomic_list_concat(RevHistoryAtoms, '->', OutputAtomPath),
    format("~q\n",[OutputAtomPath]),
    true.


print_list(A):-
    maplist(term_to_atom,A,RevHistoryAtoms),
    atomic_list_concat(RevHistoryAtoms, '--', OutputAtomPath),
    format("~q\n",[OutputAtomPath]),
    true.

filter_states([], []).
filter_states([H|T], ValidMoves) :-
    end_state(H),
    ValidMoves = [H];
    \+ end_state(H),
    filter_states(T, ValidMoves1),
    ValidMoves = [H|ValidMoves1].

get_not_visited([H|T], NextState, ValidMoves, Visited) :-
    NextState = H,
    % format("NextState is :~q ",NextState),
    \+ member(H, Visited);
    get_not_visited(T, NextState, ValidMoves, Visited).

dfs_search(Path, FinalPath, _) :-
    last(Path, LastState),
    FinalPath = Path,
    end_state(LastState).

dfs_search(Path, FinalPath, Visited) :-
    last(Path, CurrentState),
    % format("Path: "),print_path(Path),
    % format("CurrentState: "),print_path([CurrentState]),
    valid_moves(CurrentState, ValidMoves),
    % format("ValidMoves: "),print_path(ValidMoves),
    % format("Visited: "),print_path(Visited),
    get_not_visited(ValidMoves,NextState,ValidMoves,Visited),
    % format("NextState: "),print_path([NextState]),

    append([  Visited, [NextState]], L), Visited_ = L,
    % print_path(Visited_),
    % format("=====================================\n"),
    dfs_search(L, FinalPath, L).


% Hàm bắt đầu giải bài toán
go :-
    dfs_search([[0,0,0,0]], FinalPath, [[0,0,0,0]]),
    format("FinalPath: "),print_path(FinalPath).

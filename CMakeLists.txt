cmake_minimum_required(VERSION 3.15)
project(flight_new)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread" )

add_executable(flight_new main.cpp AssignCommand.cpp AssignCommand.h BinaryExpression.h Command.h CommandExpression.h ConditionParser.cpp ConditionParser.h ConnectControlClient.h ConnectControlClient.cpp DataReaderServer.cpp DataReaderServer.h DefineVarCommand.cpp DefineVarCommand.h Div.h Evaluator.cpp Evaluator.h Expression.h IfCommand.cpp IfCommand.h Parser.cpp Parser.h LoopCommand.cpp LoopCommand.h LRarrow.cpp LRarrow.h Lexer.cpp Lexer.h Minus.h Modifier.cpp Modifier.h Mul.h Number.h OpenServerCommand.h Plus.h PrintCommand.cpp PrintCommand.h RLarrow.cpp RLarrow.h SleepCommand.cpp SleepCommand.h StringHelper.cpp StringHelper.h)
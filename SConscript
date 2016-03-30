env = Environment()

env.ParseConfig('pkg-config --cflags --libs openssl')

env.Append(CXXFLAGS=['-std=c++14', '-Wall', '-g'])

env.Append(CPPPATH=['/usr/local/include'])


env.Append(LIBS=['pthread', 'boost_system', 'boost_regex', 'boost_thread', 'boost_filesystem', 'boost_program_options', 'boost_coroutine', 'boost_context', 'apr-1', 'aprutil-1', 'log4cxx', 'libmysqlcppconn'])


env.Append(LIBPATH=['/usr/local/lib'])

env.Program('x', ['main.cpp', 'json11.cpp', 'handler_md.cpp', 'slot_md.cpp', 'slot.cpp', 'handler/user_info.cpp', 'handler/slot_info.cpp'])

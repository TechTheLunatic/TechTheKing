#include <boost/bind.hpp>
#include <boost/thread.hpp>

class Thread{
	public:
		void ouvrirThread();
		void fermerThread();
		virtual ~Thread();
	protected:
		Thread();
		virtual void thread()=0;
	protected:
		boost::thread* m_thread;
};
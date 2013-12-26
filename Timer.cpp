#include "Timer.h"

#include <Windows.h>

namespace Net
{

timer::timer(unsigned long m_sec)
	: last_setted_id_(0)
	, ms_delay_(m_sec)
{
}

timer::~timer()
{
	stop();
	members_.clear();
}

void timer::add_member(i_timer_member *member)
{
	boost::unique_lock<boost::shared_mutex> locker(mutex_);

	if (member && member->id_ == 0)
	{
		last_setted_id_++;
		member->id_ = last_setted_id_;
		members_.insert(std::make_pair(last_setted_id_, member));
	}
}

void timer::remove_member(i_timer_member *member)
{
	if (!member)
	{
		return;
	}

	stop();

	boost::unique_lock<boost::shared_mutex> locker(mutex_);

	std::map<int, i_timer_member*>::iterator iter =
		members_.find(member->id_);
	if (iter != members_.end())
	{
		members_.erase(iter);
	}

	locker.unlock();

	start();
}

void timer::process()
{
	while (!thread_->need_stop())
	{
		//!fixme
		Sleep(ms_delay_);

		boost::unique_lock<boost::shared_mutex> locker(mutex_);

		std::map<int, i_timer_member*>::iterator iter =
			members_.begin();
		for (; iter != members_.end(); ++iter)
		{
			if (iter->second)
			{
				iter->second->process();
			}
		}
	}
}

void timer::start()
{
	if (!thread_.get())
	{
		thread_.reset(new ThreadWithStop(boost::bind(&timer::process, this)));
		thread_->start();
	}
}

void timer::stop()
{
	thread_->stop();
	thread_->join();
	thread_.reset();
}

} // namespace Net
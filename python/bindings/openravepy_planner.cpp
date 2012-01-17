// -*- coding: utf-8 -*-
// Copyright (C) 2006-2011 Rosen Diankov <rosen.diankov@gmail.com>
//
// This file is part of OpenRAVE.
// OpenRAVE is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#define NO_IMPORT_ARRAY
#include "openravepy_int.h"

class PyPlannerBase : public PyInterfaceBase
{
protected:
    PlannerBasePtr _pplanner;
public:
    class PyPlannerParameters
    {
        PlannerBase::PlannerParametersPtr _paramswrite;
        PlannerBase::PlannerParametersConstPtr _paramsread;
public:
        PyPlannerParameters() {
            _paramswrite.reset(new PlannerBase::PlannerParameters());
            _paramsread = _paramswrite;
        }
        PyPlannerParameters(boost::shared_ptr<PyPlannerParameters> pyparameters) {
            _paramswrite.reset(new PlannerBase::PlannerParameters());
            if( !!pyparameters ) {
                _paramswrite->copy(pyparameters->GetParameters());
            }
            _paramsread = _paramswrite;
        }
        PyPlannerParameters(PlannerBase::PlannerParametersPtr params) : _paramswrite(params), _paramsread(params) {
        }
        PyPlannerParameters(PlannerBase::PlannerParametersConstPtr params) : _paramsread(params) {
        }
        virtual ~PyPlannerParameters() {
        }

        PlannerBase::PlannerParametersConstPtr GetParameters() const {
            return _paramsread;
        }

        void SetRobotActiveJoints(PyRobotBasePtr robot)
        {
            if( !_paramswrite ) {
                throw OPENRAVE_EXCEPTION_FORMAT0("PlannerParameters needs to be non-const",ORE_Failed);
            }
            _paramswrite->SetRobotActiveJoints(openravepy::GetRobot(robot));
        }

        string __repr__() {
            return boost::str(boost::format("<PlannerParameters(dof=%d)>")%_paramsread->GetDOF());
        }
        string __str__() {
            stringstream ss;
            ss << std::setprecision(std::numeric_limits<dReal>::digits10+1);         /// have to do this or otherwise precision gets lost
            ss << *_paramsread << endl;
            return ss.str();
        }
        bool __eq__(boost::shared_ptr<PyPlannerParameters> p) {
            return !!p && _paramsread == p->_paramsread;
        }
        bool __ne__(boost::shared_ptr<PyPlannerParameters> p) {
            return !p || _paramsread != p->_paramsread;
        }
    };

    typedef boost::shared_ptr<PyPlannerParameters> PyPlannerParametersPtr;
    typedef boost::shared_ptr<PyPlannerParameters const> PyPlannerParametersConstPtr;

    PyPlannerBase(PlannerBasePtr pplanner, PyEnvironmentBasePtr pyenv) : PyInterfaceBase(pplanner, pyenv), _pplanner(pplanner) {
    }
    virtual ~PyPlannerBase() {
    }

    bool InitPlan(PyRobotBasePtr pbase, PyPlannerParametersPtr pparams)
    {
        return _pplanner->InitPlan(openravepy::GetRobot(pbase),pparams->GetParameters());
    }

    bool InitPlan(PyRobotBasePtr pbase, const string& params)
    {
        stringstream ss(params);
        return _pplanner->InitPlan(openravepy::GetRobot(pbase),ss);
    }

    bool PlanPath(PyTrajectoryBasePtr pytraj)
    {
        return _pplanner->PlanPath(openravepy::GetTrajectory(pytraj));
    }

    PyPlannerParametersPtr GetParameters() const
    {
        PlannerBase::PlannerParametersConstPtr params = _pplanner->GetParameters();
        if( !params ) {
            return PyPlannerParametersPtr();
        }
        return PyPlannerParametersPtr(new PyPlannerParameters(params));
    }

    PlannerBasePtr GetPlanner()
    {
        return _pplanner;
    }
};

namespace openravepy {

PlannerBasePtr GetPlanner(PyPlannerBasePtr pyplanner)
{
    return !pyplanner ? PlannerBasePtr() : pyplanner->GetPlanner();
}

PyInterfaceBasePtr toPyPlanner(PlannerBasePtr pplanner, PyEnvironmentBasePtr pyenv)
{
    return !pplanner ? PyInterfaceBasePtr() : PyInterfaceBasePtr(new PyPlannerBase(pplanner,pyenv));
}

PlannerBase::PlannerParametersConstPtr GetPlannerParametersConst(object o)
{
    extract<PyPlannerBase::PyPlannerParametersPtr> pyparams(o);
    if( pyparams.check() ) {
        return ((PyPlannerBase::PyPlannerParametersPtr)pyparams)->GetParameters();
    }
    return PlannerBase::PlannerParametersPtr();
}

PyPlannerBasePtr RaveCreatePlanner(PyEnvironmentBasePtr pyenv, const std::string& name)
{
    PlannerBasePtr p = OpenRAVE::RaveCreatePlanner(GetEnvironment(pyenv), name);
    if( !p ) {
        return PyPlannerBasePtr();
    }
    return PyPlannerBasePtr(new PyPlannerBase(p,pyenv));
}

void init_openravepy_planner()
{
    {
        bool (PyPlannerBase::*InitPlan1)(PyRobotBasePtr, PyPlannerBase::PyPlannerParametersPtr) = &PyPlannerBase::InitPlan;
        bool (PyPlannerBase::*InitPlan2)(PyRobotBasePtr, const string &) = &PyPlannerBase::InitPlan;
        scope planner = class_<PyPlannerBase, boost::shared_ptr<PyPlannerBase>, bases<PyInterfaceBase> >("Planner", DOXY_CLASS(PlannerBase), no_init)
                        .def("InitPlan",InitPlan1,args("robot","params"), DOXY_FN(PlannerBase,InitPlan "RobotBasePtr; PlannerParametersConstPtr"))
                        .def("InitPlan",InitPlan2,args("robot","xmlparams"), DOXY_FN(PlannerBase,InitPlan "RobotBasePtr; std::istream"))
                        .def("PlanPath",&PyPlannerBase::PlanPath,args("traj"), DOXY_FN(PlannerBase,PlanPath))
                        .def("GetParameters",&PyPlannerBase::GetParameters, DOXY_FN(PlannerBase,GetParameters))
        ;

        class_<PyPlannerBase::PyPlannerParameters, PyPlannerBase::PyPlannerParametersPtr >("PlannerParameters", DOXY_CLASS(PlannerBase::PlannerParameters))
        .def(init<>())
        .def(init<PyPlannerBase::PyPlannerParametersPtr>(args("parameters")))
        .def("SetRobotActiveJoints",&PyPlannerBase::PyPlannerParameters::SetRobotActiveJoints, args("robot"), DOXY_FN(PlannerBase::PlannerParameters, SetRobotActiveJoints))
        .def("__str__",&PyPlannerBase::PyPlannerParameters::__str__)
        .def("__repr__",&PyPlannerBase::PyPlannerParameters::__repr__)
        .def("__eq__",&PyPlannerBase::PyPlannerParameters::__eq__)
        .def("__ne__",&PyPlannerBase::PyPlannerParameters::__ne__)
        ;
    }

    object plannerstatus = enum_<PlannerStatus>("PlannerStatus" DOXY_ENUM(PlannerStatus))
                           .value("Failed",PS_Failed)
                           .value("HasSolution",PS_HasSolution)
                           .value("Interrupted",PS_Interrupted)
                           .value("InterruptedWithSolution",PS_InterruptedWithSolution)
    ;

    def("RaveCreatePlanner",openravepy::RaveCreatePlanner,args("env","name"),DOXY_FN1(RaveCreatePlanner));
}

}

//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PenaltyEqualValueConstraint.h"

registerMooseObject("MooseApp", PenaltyEqualValueConstraint);

InputParameters
PenaltyEqualValueConstraint::validParams()
{
  InputParameters params = ADMortarConstraint::validParams();
  params.addClassDescription(
      "PenaltyEqualValueConstraint enforces solution continuity between secondary and "
      "primary sides of a mortar interface using a penalty approach (no Lagrange multipliers "
      "needed)");
  params.addRequiredRangeCheckedParam<Real>(
      "penalty_value",
      "penalty_value>0",
      "Penalty value used to impose a generalized force capturing the mortar constraint equation");
  return params;
}

PenaltyEqualValueConstraint::PenaltyEqualValueConstraint(const InputParameters & parameters)
  : ADMortarConstraint(parameters), _penalty_value(getParam<Real>("penalty_value"))
{
}

ADReal
PenaltyEqualValueConstraint::computeQpResidual(Moose::MortarType mortar_type)
{
  switch (mortar_type)
  {
    case Moose::MortarType::Secondary:
    {
      auto residual =
          -(_u_primary[_qp] - _u_secondary[_qp]) * _penalty_value * _test_secondary[_i][_qp];

      return residual;
    }

    case Moose::MortarType::Primary:
    {
      auto residual =
          (_u_primary[_qp] - _u_secondary[_qp]) * _penalty_value * _test_primary[_i][_qp];

      return residual;
    }

    default:
      return 0;
  }
}

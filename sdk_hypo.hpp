#ifndef SDK_HYPO_HPP
#define SDK_HYPO_HPP

/*!
 * \file sdk_hypo.hpp
 * \brief Hypothesis
 * \author Christophe Haen
 * \version 0.1
 */
 
/*! \class sdk_hypo
 * \brief Class representing a hypothesis
 *
 *  Hypothesis one has to make in case of multiple choice
 */
class sdk_hypo{
public:
  int x; /*! index on the line */
  int y; /*! index on the column */
  vector<int> tried; /*! List of values already tried */

  /*!
	 *  \brief Constructor
	 *
	 *  Constructor
	 *
	 *  \param x : index on the line
	 *  \param y : index on the column
	 */
  sdk_hypo(int x = 0, int y = 0){
    this->x = x;
    this->y = y;
  }
};

#endif
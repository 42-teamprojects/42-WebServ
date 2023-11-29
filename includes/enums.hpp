/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yelaissa <yelaissa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:32:19 by yelaissa          #+#    #+#             */
/*   Updated: 2023/11/29 10:32:30 by yelaissa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef enum e_HttpStatusCode {
    OK = 200,
    MovedPermanently = 301,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    Conflict = 409,
    MethodNotAllowed = 405,
    RequestURITooLong = 414,
    RequestEntityTooLarge = 413,
    NotImplemented = 501,
    ServerError = 500,
    NoContent = 204
} HttpStatusCode;

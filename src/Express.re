type complete;

module Error = {
  type t = exn;

  /*** Error type */
  [@mel.send] [@mel.return null_undefined_to_opt]
  external message: Js.Exn.t => option(string) = "message";
  [@mel.send] [@mel.return null_undefined_to_opt]
  external name: Js.Exn.t => option(string) = "name";
};

module Request = {
  type t;
  type params = Js.Dict.t(Js.Json.t);
  [@mel.get] external params: t => params = "params";

  /*** [params request] return the JSON object filled with the
       request parameters */
  external asJsonObject: t => Js.Dict.t(Js.Json.t) = "%identity";

  /*** [asJsonObject request] casts a [request] to a JSON object. It is
       common in Express application to use the Request object as a
       placeholder to maintain state through the various middleware which
       are executed. */
  [@mel.get] external baseUrl: t => string = "baseUrl";
  [@mel.get] external body_: t => 'a = "body";

  /*** When using the json body-parser middleware and receiving a request with a
       content type of "application/json", this property is a Js.Json.t that
       contains the body sent by the request. **/
  [@mel.get] [@mel.return null_undefined_to_opt]
  external bodyJSON: t => option(Js.Json.t) = "body";

  /*** When using the raw body-parser middleware and receiving a request with a
       content type of "application/octet-stream", this property is a
       Node_buffer.t that contains the body sent by the request. **/
  [@mel.get] [@mel.return null_undefined_to_opt]
  external bodyRaw: t => option(Node.Buffer.t) = "body";

  /*** When using the text body-parser middleware and receiving a request with a
       content type of "text/plain", this property is a string that
       contains the body sent by the request. **/
  let bodyText: t => option(string) =
    req => {
      let body: string = body_(req);
      if (Js.Json.test(body, Js.Json.String)) {
        Some(body);
      } else {
        None;
      };
    };
  let decodeStringDict = json =>
    Option.bind(
      Js.Json.decodeObject(json),
      obj => {
        let source: Js.Dict.t(string) = Obj.magic(obj);
        let allStrings =
          Js.Dict.values(source)
          |> Array.fold_left(
               (prev, value) => prev && Js.Json.test(value, Js.Json.String),
               true,
             );
        if (allStrings) {
          Some(source);
        } else {
          None;
        };
      },
    );
  let bodyURLEncoded: t => option(Js.Dict.t(string)) =
    req => {
      let body: Js.Json.t = body_(req);
      decodeStringDict(body);
    };

  /*** When using the urlencoded body-parser middleware and receiving a request
       with a content type of "application/x-www-form-urlencoded", this property
       is a Js.Dict.t string that contains the body sent by the request. **/
  [@mel.get] [@mel.return null_undefined_to_opt]
  external cookies: t => option(Js.Dict.t(Js.Json.t)) = "cookies";

  /*** When using cookie-parser middleware, this property is an object
       that contains cookies sent by the request. If the request contains
       no cookies, it defaults to {}.*/
  [@mel.get] [@mel.return null_undefined_to_opt]
  external signedCookies: t => option(Js.Dict.t(Js.Json.t)) =
    "signedCookies";

  /*** When using cookie-parser middleware, this property contains signed cookies
       sent by the request, unsigned and ready for use. Signed cookies reside in
       a different object to show developer intent; otherwise, a malicious attack
       could be placed on req.cookie values (which are easy to spoof).
       Note that signing a cookie does not make it “hidden” or encrypted;
       but simply prevents tampering (because the secret used to
       sign is private). **/
  [@mel.get] external hostname: t => string = "hostname";

  /*** [hostname request] Contains the hostname derived from the Host
       HTTP header.*/
  [@mel.get] external ip: t => string = "ip";

  /*** [ip request] Contains the remote IP address of the request.*/
  [@mel.get] external fresh: t => bool = "fresh";

  /*** [fresh request] returns [true] whether the request is "fresh" */
  [@mel.get] external stale: t => bool = "stale";

  /*** [stale request] returns [true] whether the request is "stale"*/
  [@mel.get] external methodRaw: t => string = "method";
  type httpMethod =
    | Get
    | Post
    | Put
    | Delete
    | Head
    | Options
    | Trace
    | Connect
    | Patch;
  let httpMethod: t => httpMethod =
    req =>
      switch (methodRaw(req)) {
      | "GET" => Get
      | "POST" => Post
      | "PUT" => Put
      | "PATCH" => Patch
      | "DELETE" => Delete
      | "HEAD" => Head
      | "OPTIONS" => Options
      | "TRACE" => Trace
      | "CONNECT" => Connect
      | s => failwith("Express.Request.method_ Unexpected method: " ++ s)
      };

  /*** [method_ request] return a string corresponding to the HTTP
       method of the request: GET, POST, PUT, and so on */
  [@mel.get] external originalUrl: t => string = "originalUrl";

  /*** [originalUrl request] returns the original url. See
       https://expressjs.com/en/4x/api.html#req.originalUrl */
  [@mel.get] external path: t => string = "path";

  /*** [path request] returns the path part of the request URL.*/
  type protocol =
    | Http
    | Https;
  let protocol: t => protocol =
    req => {
      module Raw = {
        [@mel.get] external protocol: t => string = "protocol";
      };
      switch (Raw.protocol(req)) {
      | "http" => Http
      | "https" => Https
      | s => failwith("Express.Request.protocol, Unexpected protocol: " ++ s)
      };
    };

  /*** [protocol request] returns the request protocol string: either http
       or (for TLS requests) https. */
  [@mel.get] external secure: t => bool = "secure";

  /*** [secure request] returns [true] if a TLS connection is established */
  [@mel.get] external query: t => Js.Dict.t(Js.Json.t) = "query";

  /*** [query request] returns an object containing a property for each
       query string parameter in the route. If there is no query string,
       it returns the empty object, {} */
  let accepts: (array(string), t) => option(string) =
    (types, req) => {
      module Raw = {
        [@mel.send]
        external accepts: (t, array(string)) => Js.Json.t = "accepts";
      };
      let ret = Raw.accepts(req, types);
      let tagged_t = Js.Json.classify(ret);
      switch (tagged_t) {
      | JSONString(x) => Some(x)
      | _ => None
      };
    };

  /*** [acceptsRaw accepts types] checks if the specified content types
       are acceptable, based on the request's Accept HTTP header field.
       The method returns the best match, or if none of the specified
       content types is acceptable, returns [false] */
  let acceptsCharsets: (array(string), t) => option(string) =
    (types, req) => {
      module Raw = {
        [@mel.send]
        external acceptsCharsets: (t, array(string)) => Js.Json.t =
          "acceptsCharsets";
      };
      let ret = Raw.acceptsCharsets(req, types);
      let tagged_t = Js.Json.classify(ret);
      switch (tagged_t) {
      | JSONString(x) => Some(x)
      | _ => None
      };
    };
  [@mel.send.pipe: t] [@mel.return null_undefined_to_opt]
  external get: string => option(string) = "get";

  /*** [get return field] returns the specified HTTP request header
       field (case-insensitive match) */
  [@mel.get] external xhr: t => bool = "xhr";
  /*** [xhr request] returns [true] if the request’s X-Requested-With
       header field is "XMLHttpRequest", indicating that the request was
       issued by a client library such as jQuery */
};

module Response = {
  type t;
  module StatusCode = {
    [@deriving jsConverter]
    type t =
      | [@mel.as 200] Ok
      | [@mel.as 201] Created
      | [@mel.as 202] Accepted
      | [@mel.as 203] NonAuthoritativeInformation
      | [@mel.as 204] NoContent
      | [@mel.as 205] ResetContent
      | [@mel.as 206] PartialContent
      | [@mel.as 207] MultiStatus
      | [@mel.as 208] AleadyReported
      | [@mel.as 226] IMUsed
      | [@mel.as 300] MultipleChoices
      | [@mel.as 301] MovedPermanently
      | [@mel.as 302] Found
      | [@mel.as 303] SeeOther
      | [@mel.as 304] NotModified
      | [@mel.as 305] UseProxy
      | [@mel.as 306] SwitchProxy
      | [@mel.as 307] TemporaryRedirect
      | [@mel.as 308] PermanentRedirect
      | [@mel.as 400] BadRequest
      | [@mel.as 401] Unauthorized
      | [@mel.as 402] PaymentRequired
      | [@mel.as 403] Forbidden
      | [@mel.as 404] NotFound
      | [@mel.as 405] MethodNotAllowed
      | [@mel.as 406] NotAcceptable
      | [@mel.as 407] ProxyAuthenticationRequired
      | [@mel.as 408] RequestTimeout
      | [@mel.as 409] Conflict
      | [@mel.as 410] Gone
      | [@mel.as 411] LengthRequired
      | [@mel.as 412] PreconditionFailed
      | [@mel.as 413] PayloadTooLarge
      | [@mel.as 414] UriTooLong
      | [@mel.as 415] UnsupportedMediaType
      | [@mel.as 416] RangeNotSatisfiable
      | [@mel.as 417] ExpectationFailed
      | [@mel.as 418] ImATeapot
      | [@mel.as 421] MisdirectedRequest
      | [@mel.as 422] UnprocessableEntity
      | [@mel.as 423] Locked
      | [@mel.as 424] FailedDependency
      | [@mel.as 426] UpgradeRequired
      | [@mel.as 428] PreconditionRequired
      | [@mel.as 429] TooManyRequests
      | [@mel.as 431] RequestHeaderFieldsTooLarge
      | [@mel.as 451] UnavailableForLegalReasons
      | [@mel.as 500] InternalServerError
      | [@mel.as 501] NotImplemented
      | [@mel.as 502] BadGateway
      | [@mel.as 503] ServiceUnavailable
      | [@mel.as 504] GatewayTimeout
      | [@mel.as 505] HttpVersionNotSupported
      | [@mel.as 506] VariantAlsoNegotiates
      | [@mel.as 507] InsufficientStorage
      | [@mel.as 508] LoopDetected
      | [@mel.as 510] NotExtended
      | [@mel.as 511] NetworkAuthenticationRequired;
    let fromInt = tFromJs;
    let toInt = tToJs;
  };
  [@mel.send.pipe: t]
  external cookie_: (string, Js.Json.t, 'a) => unit = "cookie";
  [@mel.send.pipe: t]
  external clearCookie_: (string, 'a) => unit = "clearCookie";
  [@deriving jsConverter]
  type sameSite = [
    | [@mel.as "lax"] `Lax
    | [@mel.as "strict"] `Strict
    | [@mel.as "none"] `None
  ];
  external toDict: 'a => Js.Dict.t(Js.nullable('b)) = "%identity";
  let filterKeys = obj => {
    let result = toDict(obj);
    result
    |> Js.Dict.entries
    |> Js.Array.filter(~f=((_key, value)) =>
         !Js.Nullable.isNullable(value)
       )
    |> Js.Dict.fromArray;
  };
  let cookie =
      (
        ~name,
        ~maxAge=?,
        ~expiresGMT=?,
        ~httpOnly=?,
        ~secure=?,
        ~signed=?,
        ~path=?,
        ~sameSite: option(sameSite)=?,
        ~domain=?,
        value,
        response,
      ) => {
    cookie_(
      name,
      value,
      {
        "maxAge": maxAge |> Js.Nullable.fromOption,
        "expires": expiresGMT |> Js.Nullable.fromOption,
        "path": path |> Js.Nullable.fromOption,
        "httpOnly": httpOnly |> Js.Nullable.fromOption,
        "secure": secure |> Js.Nullable.fromOption,
        "sameSite":
          sameSite
          |> Option.map(x => sameSiteToJs(x))
          |> Js.Nullable.fromOption,
        "signed": signed |> Js.Nullable.fromOption,
        "domain": domain |> Js.Nullable.fromOption,
      }
      |> filterKeys,
      response,
    );
    response;
  };
  let clearCookie =
      (
        ~name,
        ~httpOnly=?,
        ~secure=?,
        ~signed=?,
        ~path="/",
        ~sameSite: option(sameSite)=?,
        response,
      ) => {
    clearCookie_(
      name,
      {
        "maxAge": Js.Nullable.undefined,
        "expires": Js.Nullable.undefined,
        "path": path,
        "httpOnly": httpOnly |> Js.Nullable.fromOption,
        "secure": secure |> Js.Nullable.fromOption,
        "sameSite":
          sameSite
          |> Option.map(x => sameSiteToJs(x))
          |> Js.Nullable.fromOption,
        "signed": signed |> Js.Nullable.fromOption,
      }
      |> filterKeys,
      response,
    );
    response;
  };
  [@mel.send.pipe: t] external sendFile: (string, 'a) => complete = "sendFile";
  [@mel.send.pipe: t] external sendString: string => complete = "send";
  [@mel.send.pipe: t] external sendJson: Js.Json.t => complete = "json";
  [@mel.send.pipe: t] external sendBuffer: Node.Buffer.t => complete = "send";
  [@mel.send.pipe: t] external sendArray: array('a) => complete = "send";
  [@mel.send.pipe: t] external sendRawStatus: int => complete = "sendStatus";
  let sendStatus = statusCode => sendRawStatus(StatusCode.toInt(statusCode));
  [@mel.send.pipe: t] external rawStatus: int => t = "status";
  let status = statusCode => rawStatus(StatusCode.toInt(statusCode));
  [@mel.send.pipe: t] [@ocaml.deprecated "Use sendJson instead`"]
  external json: Js.Json.t => complete = "json";
  [@mel.send.pipe: t]
  external redirectCode: (int, string) => complete = "redirect";
  [@mel.send.pipe: t] external redirect: string => complete = "redirect";
  [@mel.send.pipe: t] external setHeader: (string, string) => t = "set";
  [@mel.send.pipe: t] external setType: string => t = "type";
  [@mel.send.pipe: t] external setLinks: Js.Dict.t(string) => t = "links";
  [@mel.send.pipe: t] external end_: complete = "end";
  [@mel.send.pipe: t]
  external render: (string, Js.Dict.t(string), 'a) => complete = "render";
};

module Next: {
  type content;
  type t = (Js.undefined(content), Response.t) => complete;
  let middleware: Js.undefined(content);

  /*** value to use as [next] callback argument to invoke the next
       middleware */
  let route: Js.undefined(content);

  /*** value to use as [next] callback argument to skip middleware
       processing for the current route.*/
  let error: Error.t => Js.undefined(content);
  /*** [error e] returns the argument for [next] callback to be propagate
       error [e] through the chain of middleware. */
} = {
  type content;
  type t = (Js.undefined(content), Response.t) => complete;
  let middleware = Js.undefined;
  external castToContent: 'a => content = "%identity";
  let route = Js.Undefined.return(castToContent("route"));
  let error = (e: Error.t) => Js.Undefined.return(castToContent(e));
};

module ByteLimit = {
  [@deriving accessors]
  type t =
    | B(int)
    | Kb(float)
    | Mb(float)
    | Gb(float);
  let toBytes =
    fun
    | Some(B(b)) => Js.Nullable.return(b)
    | Some(Kb(kb)) => Js.Nullable.return(int_of_float(1024.0 *. kb))
    | Some(Mb(mb)) =>
      Js.Nullable.return(int_of_float(1024.0 *. 1024.0 *. mb))
    | Some(Gb(gb)) =>
      Js.Nullable.return(int_of_float(1024.0 *. 1024.0 *. 1024.0 *. gb))
    | None => Js.Nullable.undefined;
};

module Middleware = {
  type next = Next.t;
  type t;
  type jsonOptions = {
    .
    "inflate": bool,
    "strict": bool,
    "limit": Js.nullable(int),
  };
  type urlEncodedOptions = {
    .
    "extended": bool,
    "inflate": bool,
    "limit": Js.nullable(int),
    "parameterLimit": Js.nullable(int),
  };
  type textOptions = {
    .
    "defaultCharset": string,
    "inflate": bool,
    "type": string,
    "limit": Js.Nullable.t(int),
  };
  type rawOptions = {
    .
    "inflate": bool,
    "type": string,
    "limit": Js.Nullable.t(int),
  };
  [@mel.module "express"] external json_: jsonOptions => t = "json";
  [@mel.module "express"]
  external urlencoded_: urlEncodedOptions => t = "urlencoded";
  let json = (~inflate=true, ~strict=true, ~limit=?, ()) =>
    json_({
      "inflate": inflate,
      "strict": strict,
      "limit": ByteLimit.toBytes(limit),
    });
  [@mel.module "body-parser"] external text_: textOptions => t = "text";
  let text =
      (
        ~defaultCharset="utf-8",
        ~fileType="text/plain",
        ~inflate=true,
        ~limit: option(ByteLimit.t)=?,
        (),
      ) =>
    text_({
      "defaultCharset": defaultCharset,
      "type": fileType,
      "limit": ByteLimit.toBytes(limit),
      "inflate": inflate,
    });
  let urlencoded =
      (~extended=false, ~inflate=true, ~limit=?, ~parameterLimit=?, ()) =>
    urlencoded_({
      "inflate": inflate,
      "extended": extended,
      "parameterLimit": parameterLimit |> Js.Nullable.fromOption,
      "limit": ByteLimit.toBytes(limit),
    });
  [@mel.module "body-parser"] external raw_: rawOptions => t = "raw";
  let raw =
      (
        ~inflate=true,
        ~fileType="application/octet-stream",
        ~limit: option(ByteLimit.t)=?,
        (),
      ) =>
    raw_({
      "type": fileType,
      "limit": ByteLimit.toBytes(limit),
      "inflate": inflate,
    });
  module type S = {
    type f;
    type errorF;
    let from: f => t;
    /* Generate the common Middleware binding function for a given
     * type. This Functor is used for the Router and App classes. */
    let fromError: errorF => t;
  };
  module type ApplyMiddleware = {
    type f;
    let apply: (f, next, Request.t, Response.t) => unit;
    type errorF;
    let applyWithError: (errorF, next, Error.t, Request.t, Response.t) => unit;
  };
  module Make =
         (A: ApplyMiddleware)
         : (S with type f = A.f and type errorF = A.errorF) => {
    type f = A.f;
    external unsafeFrom: 'a => t = "%identity";
    let from = middleware => {
      let aux = (next, content, _) => next(content);
      unsafeFrom((req, res, next) =>
        A.apply(middleware, aux(next), req, res)
      );
    };
    type errorF = A.errorF;
    let fromError = middleware => {
      let aux = (next, content, _) => next(content);
      unsafeFrom((err, req, res, next) =>
        A.applyWithError(middleware, aux(next), err, req, res)
      );
    };
  };
  include Make({
    type f = (next, Request.t, Response.t) => complete;
    type errorF = (next, Error.t, Request.t, Response.t) => complete;
    let apply = (f, next, req, res) =>
      (
        try(f(next, req, res)) {
        | e => next(Next.error(e), res)
        }
      )
      |> ignore;
    let applyWithError = (f, next, err, req, res) =>
      (
        try(f(next, err, req, res)) {
        | e => next(Next.error(e), res)
        }
      )
      |> ignore;
  });
};

module PromiseMiddleware =
  Middleware.Make({
    type f =
      (Middleware.next, Request.t, Response.t) => Js.Promise.t(complete);
    type errorF =
      (Middleware.next, Error.t, Request.t, Response.t) =>
      Js.Promise.t(complete);
    external castToErr: Js.Promise.error => Error.t = "%identity";
    let apply = (f, next, req, res) => {
      let promise: Js.Promise.t(complete) =
        try(f(next, req, res)) {
        | e => Js.Promise.resolve(next(Next.error(e), res))
        };
      promise
      |> Js.Promise.catch(err => {
           let err = castToErr(err);
           Js.Promise.resolve(next(Next.error(err), res));
         })
      |> ignore;
    };
    let applyWithError = (f, next, err, req, res) => {
      let promise: Js.Promise.t(complete) =
        try(f(next, err, req, res)) {
        | e => Js.Promise.resolve(next(Next.error(e), res))
        };
      promise
      |> Js.Promise.catch(err => {
           let err = castToErr(err);
           Js.Promise.resolve(next(Next.error(err), res));
         })
      |> ignore;
    };
  });

module type Routable = {
  type t;
  let use: (t, Middleware.t) => unit;
  let useWithMany: (t, array(Middleware.t)) => unit;
  let useOnPath: (t, ~path: string, Middleware.t) => unit;
  let useOnPathWithMany: (t, ~path: string, array(Middleware.t)) => unit;
  let get: (t, ~path: string, Middleware.t) => unit;
  let getWithMany: (t, ~path: string, array(Middleware.t)) => unit;
  let options: (t, ~path: string, Middleware.t) => unit;
  let optionsWithMany: (t, ~path: string, array(Middleware.t)) => unit;
  let param: (t, ~name: string, Middleware.t) => unit;
  let post: (t, ~path: string, Middleware.t) => unit;
  let postWithMany: (t, ~path: string, array(Middleware.t)) => unit;
  let put: (t, ~path: string, Middleware.t) => unit;
  let putWithMany: (t, ~path: string, array(Middleware.t)) => unit;
  let patch: (t, ~path: string, Middleware.t) => unit;
  let patchWithMany: (t, ~path: string, array(Middleware.t)) => unit;
  let delete: (t, ~path: string, Middleware.t) => unit;
  let deleteWithMany: (t, ~path: string, array(Middleware.t)) => unit;
};

module MakeBindFunctions = (T: {
                              type t;
                            }) : (Routable with type t = T.t) => {
  type t = T.t;
  [@mel.send] external use: (T.t, Middleware.t) => unit = "use";
  [@mel.send]
  external useWithMany: (T.t, array(Middleware.t)) => unit = "use";
  [@mel.send]
  external useOnPath: (T.t, ~path: string, Middleware.t) => unit = "use";
  [@mel.send]
  external useOnPathWithMany:
    (T.t, ~path: string, array(Middleware.t)) => unit =
    "use";
  [@mel.send] external get: (T.t, ~path: string, Middleware.t) => unit = "get";
  [@mel.send]
  external getWithMany: (T.t, ~path: string, array(Middleware.t)) => unit =
    "get";
  [@mel.send]
  external options: (T.t, ~path: string, Middleware.t) => unit = "options";
  [@mel.send]
  external optionsWithMany: (T.t, ~path: string, array(Middleware.t)) => unit =
    "options";
  [@mel.send]
  external param: (T.t, ~name: string, Middleware.t) => unit = "param";
  [@mel.send]
  external post: (T.t, ~path: string, Middleware.t) => unit = "post";
  [@mel.send]
  external postWithMany: (T.t, ~path: string, array(Middleware.t)) => unit =
    "post";
  [@mel.send] external put: (T.t, ~path: string, Middleware.t) => unit = "put";
  [@mel.send]
  external putWithMany: (T.t, ~path: string, array(Middleware.t)) => unit =
    "put";
  [@mel.send]
  external patch: (T.t, ~path: string, Middleware.t) => unit = "patch";
  [@mel.send]
  external patchWithMany: (T.t, ~path: string, array(Middleware.t)) => unit =
    "patch";
  [@mel.send]
  external delete: (T.t, ~path: string, Middleware.t) => unit = "delete";
  [@mel.send]
  external deleteWithMany: (T.t, ~path: string, array(Middleware.t)) => unit =
    "delete";
};

module Router = {
  include MakeBindFunctions({
    type t;
  });
  type routerArgs = {
    .
    "caseSensitive": bool,
    "mergeParams": bool,
    "strict": bool,
  };
  [@mel.module "express"] external make_: routerArgs => t = "Router";
  let make = (~caseSensitive=false, ~mergeParams=false, ~strict=false, ()) =>
    make_({
      "caseSensitive": caseSensitive,
      "mergeParams": mergeParams,
      "strict": strict,
    });
  external asMiddleware: t => Middleware.t = "%identity";
};

let router = Router.make;

module HttpServer = {
  type t;
  [@mel.send]
  external on:
    (
      t,
      [@mel.string] [
        | `request((Request.t, Response.t) => unit)
        | `close(unit => unit)
      ]
    ) =>
    unit =
    "on";
};

module App = {
  include MakeBindFunctions({
    type t;
  });
  let useRouter = (app, router) => Router.asMiddleware(router) |> use(app);
  let useRouterOnPath = (app, ~path, router) =>
    Router.asMiddleware(router) |> useOnPath(app, ~path);
  [@mel.module] external make: unit => t = "express";

  /*** [make ()] creates an instance of the App class. */
  external asMiddleware: t => Middleware.t = "%identity";

  /*** [asMiddleware app] casts an App instance to a Middleware type */
  [@mel.send]
  external listen_:
    (
      t,
      int,
      string,
      [@mel.uncurry] (Js.Nullable.t(Js.Exn.t) => unit)
    ) =>
    HttpServer.t =
    "listen";
  let listen = (app, ~port=3000, ~hostname="0.0.0.0", ~onListen=_ => (), ()) =>
    listen_(app, port, hostname, onListen);
  [@mel.send] external disable: (t, ~name: string) => unit = "disable";
  [@mel.send] external set: (t, string, string) => unit = "set";
};

let express = App.make;

/*** [express ()] creates an instance of the App class.
     Alias for [App.make ()] */
module Static = {
  type options;
  type stat;
  let defaultOptions: unit => options =
    () => (Obj.magic(Js.Obj.empty()): options);
  [@mel.set] external dotfiles: (options, string) => unit = "dotfiles";
  [@mel.set] external etag: (options, bool) => unit = "etag";
  [@mel.set]
  external extensions: (options, array(string)) => unit = "extensions";
  [@mel.set] external fallthrough: (options, bool) => unit = "fallthrough";
  [@mel.set] external immutable: (options, bool) => unit = "immutable";
  [@mel.set] external indexBool: (options, bool) => unit = "index";
  [@mel.set] external indexString: (options, string) => unit = "index";
  [@mel.set] external lastModified: (options, bool) => unit = "lastModified";
  [@mel.set] external maxAge: (options, int) => unit = "maxAge";
  [@mel.set] external redirect: (options, bool) => unit = "redirect";
  [@mel.set]
  external setHeaders: (options, (Request.t, string, stat) => unit) => unit =
    "setHeaders";

  type t;
  [@mel.module "express"] external make: (string, options) => t = "static";

  /*** [make directory] creates a static middleware for [directory] */
  external asMiddleware: t => Middleware.t = "%identity";
  /*** [asMiddleware static] casts [static] to a Middleware type */
};

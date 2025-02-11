type complete;

/** amel.ract type which ensure middleware function must either
     call the [next] function or one of the [send] function on the
     response object.

     This should be a great argument for OCaml, the type system
     prevents silly error which in this case would make the server hang */
module Error: {
  type t = exn;

  /** Error type */
  [@mel.send] [@mel.return null_undefined_to_opt]
  external message: Js.Exn.t => option(string) = "message";
  [@mel.send] [@mel.return null_undefined_to_opt]
  external name: Js.Exn.t => option(string) = "name";
};

module Request: {
  type t;
  type params = Js.Dict.t(Js.Json.t);

  /** [params request] return the JSON object filled with the
       request parameters */
  [@mel.get]
  external params: t => params = "params";

  /** [asJsonObject request] casts a [request] to a JSON object. It is
       common in Express application to use the Request object as a
       placeholder to maintain state through the various middleware which
       are executed. */
  external asJsonObject: t => Js.Dict.t(Js.Json.t) = "%identity";

  /** [baseUrl request] returns the 'baseUrl' property */
  [@mel.get]
  external baseUrl: t => string = "baseUrl";

  /** When using the json body-parser middleware and receiving a request with a
       content type of "application/json", this property is a Js.Json.t that
       contains the body sent by the request. */
  [@mel.get] [@mel.return null_undefined_to_opt]
  external bodyJSON: t => option(Js.Json.t) = "body";

  /** When using the raw body-parser middleware and receiving a request with a
       content type of "application/octet-stream", this property is a
       Node_buffer.t that contains the body sent by the request. */
  [@mel.get] [@mel.return null_undefined_to_opt]
  external bodyRaw: t => option(Node.Buffer.t) = "body";

  /** When using the text body-parser middleware and receiving a request with a
       content type of "text/plain", this property is a string that
       contains the body sent by the request. */
  let bodyText: t => option(string);

  /** When using the urlencoded body-parser middleware and receiving a request
       with a content type of "application/x-www-form-urlencoded", this property
        is a Js.Dict.t string that contains the body sent by the request. */
  let bodyURLEncoded: t => option(Js.Dict.t(string));

  /** When using cookie-parser middleware, this property is an object
       that contains cookies sent by the request. If the request contains
       no cookies, it defaults to {}.*/
  [@mel.get] [@mel.return null_undefined_to_opt]
  external cookies: t => option(Js.Dict.t(Js.Json.t)) = "cookies";

  /** When using cookie-parser middleware, this property contains signed cookies
       sent by the request, unsigned and ready for use. Signed cookies reside in
       a different object to show developer intent; otherwise, a malicious attack
       could be placed on req.cookie values (which are easy to spoof).
       Note that signing a cookie does not make it “hidden” or encrypted;
       but simply prevents tampering (because the secret used to
       sign is private). */
  [@mel.get] [@mel.return null_undefined_to_opt]
  external signedCookies: t => option(Js.Dict.t(Js.Json.t)) =
    "signedCookies";

  /** [hostname request] Contains the hostname derived from the Host
       HTTP header. */
  [@mel.get]
  external hostname: t => string = "hostname";

  /** [ip request] Contains the remote IP address of the request. */
  [@mel.get]
  external ip: t => string = "ip";

  /** [fresh request] returns [true] whether the request is "fresh" */
  [@mel.get]
  external fresh: t => bool = "fresh";

  /** [stale request] returns [true] whether the request is "stale" */
  [@mel.get]
  external stale: t => bool = "stale";

  /** [method_ request] return a string corresponding to the HTTP
       method of the request: GET, POST, PUT, and so on */
  [@mel.get]
  external methodRaw: t => string = "method";
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

  /** [method_ request] return a variant corresponding to the HTTP
       method of the request: Get, Post, Put, and so on */
  let httpMethod: t => httpMethod;

  /** [originalUrl request] returns the original url. See
       https://expressjs.com/en/4x/api.html#req.originalUrl */
  [@mel.get]
  external originalUrl: t => string = "originalUrl";

  /** [path request] returns the path part of the request URL. */
  [@mel.get]
  external path: t => string = "path";
  type protocol =
    | Http
    | Https;

  /** [protocol request] returns the request protocol string: either http
       or (for TLS requests) https. */
  let protocol: t => protocol;

  /** [secure request] returns [true] if a TLS connection is established */
  [@mel.get]
  external secure: t => bool = "secure";

  /** [query request] returns an object containing a property for each
       query string parameter in the route. If there is no query string,
       it returns the empty object, {} */
  [@mel.get]
  external query: t => Js.Dict.t(Js.Json.t) = "query";

  /** [acceptsRaw accepts types] checks if the specified content types
       are acceptable, based on the request's Accept HTTP header field.
       The method returns the best match, or if none of the specified
       content types is acceptable, returns [false] */
  let accepts: (array(string), t) => option(string);
  let acceptsCharsets: (array(string), t) => option(string);

  /** [get return field] returns the specified HTTP request header
       field (case-insensitive match) */
  [@mel.send.pipe: t] [@mel.return null_undefined_to_opt]
  external get: string => option(string) = "get";

  /** [xhr request] returns [true] if the request’s X-Requested-With
       header field is "XMLHttpRequest", indicating that the request was
       issued by a client library such as jQuery */
  [@mel.get]
  external xhr: t => bool = "xhr";
};

module Response: {
  type t;
  module StatusCode: {
    type t =
      | Ok
      | Created
      | Accepted
      | NonAuthoritativeInformation
      | NoContent
      | ResetContent
      | PartialContent
      | MultiStatus
      | AleadyReported
      | IMUsed
      | MultipleChoices
      | MovedPermanently
      | Found
      | SeeOther
      | NotModified
      | UseProxy
      | SwitchProxy
      | TemporaryRedirect
      | PermanentRedirect
      | BadRequest
      | Unauthorized
      | PaymentRequired
      | Forbidden
      | NotFound
      | MethodNotAllowed
      | NotAcceptable
      | ProxyAuthenticationRequired
      | RequestTimeout
      | Conflict
      | Gone
      | LengthRequired
      | PreconditionFailed
      | PayloadTooLarge
      | UriTooLong
      | UnsupportedMediaType
      | RangeNotSatisfiable
      | ExpectationFailed
      | ImATeapot
      | MisdirectedRequest
      | UnprocessableEntity
      | Locked
      | FailedDependency
      | UpgradeRequired
      | PreconditionRequired
      | TooManyRequests
      | RequestHeaderFieldsTooLarge
      | UnavailableForLegalReasons
      | InternalServerError
      | NotImplemented
      | BadGateway
      | ServiceUnavailable
      | GatewayTimeout
      | HttpVersionNotSupported
      | VariantAlsoNegotiates
      | InsufficientStorage
      | LoopDetected
      | NotExtended
      | NetworkAuthenticationRequired;
  };

  let cookie:
    (
      ~name: string,
      ~maxAge: int=?,
      ~expiresGMT: Js.Date.t=?,
      ~httpOnly: bool=?,
      ~secure: bool=?,
      ~signed: bool=?,
      ~path: string=?,
      ~sameSite: [ | `Lax | `Strict | `None]=?,
      ~domain: string=?,
      Js.Json.t,
      t
    ) =>
    t;

  /**
   Web browsers and other compliant clients will only clear the cookie if the given options is identical to those given to res.cookie(), excluding expires and maxAge.
    */
  let clearCookie:
    (
      ~name: string,
      ~httpOnly: bool=?,
      ~secure: bool=?,
      ~signed: bool=?,
      ~path: string=?,
      ~sameSite: [ | `Lax | `Strict | `None]=?,
      t
    ) =>
    t;

  [@mel.send.pipe: t] external sendFile: (string, 'a) => complete = "sendFile";
  [@mel.send.pipe: t] external sendString: string => complete = "send";
  [@mel.send.pipe: t] external sendJson: Js.Json.t => complete = "json";
  [@mel.send.pipe: t] external sendBuffer: Node.Buffer.t => complete = "send";
  [@mel.send.pipe: t] external sendArray: array('a) => complete = "send";
  let sendStatus: (StatusCode.t, t) => complete;
  let status: (StatusCode.t, t) => t;

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

  /** value to use as [next] callback argument to invoke the next
       middleware */
  let middleware: Js.undefined(content);

  /** value to use as [next] callback argument to skip middleware
       processing for the current route. */
  let route: Js.undefined(content);

  /** [error e] returns the argument for [next] callback to be propagate
       error [e] through the chain of middleware. */
  let error: Error.t => Js.undefined(content);
};

module ByteLimit: {
  type t =
    | B(int)
    | Kb(float)
    | Mb(float)
    | Gb(float);
  let b: int => t;
  let kb: float => t;
  let mb: float => t;
  let gb: float => t;
};

module Middleware: {
  type t;
  type next = Next.t;
  let json:
    (~inflate: bool=?, ~strict: bool=?, ~limit: ByteLimit.t=?, unit) => t;
  let text:
    (
      ~defaultCharset: string=?,
      ~fileType: string=?,
      ~inflate: bool=?,
      ~limit: ByteLimit.t=?,
      unit
    ) =>
    t;
  let raw:
    (~inflate: bool=?, ~fileType: string=?, ~limit: ByteLimit.t=?, unit) => t;
  let urlencoded:
    (
      ~extended: bool=?,
      ~inflate: bool=?,
      ~limit: ByteLimit.t=?,
      ~parameterLimit: int=?,
      unit
    ) =>
    t;
  module type S = {
    type f;
    let from: f => t;
    type errorF;
    let fromError: errorF => t;
  };
  module type ApplyMiddleware = {
    type f;
    let apply: (f, next, Request.t, Response.t) => unit;
    type errorF;
    let applyWithError: (errorF, next, Error.t, Request.t, Response.t) => unit;
  };
  module Make:
    (A: ApplyMiddleware) => S with type f = A.f and type errorF = A.errorF;
  include
    S with
      type f = (next, Request.t, Response.t) => complete and
      type errorF = (next, Error.t, Request.t, Response.t) => complete;
};

module PromiseMiddleware:
  Middleware.S with
    type f =
      (Middleware.next, Request.t, Response.t) => Js.Promise.t(complete) and
    type errorF =
      (Middleware.next, Error.t, Request.t, Response.t) =>
      Js.Promise.t(complete);

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

module MakeBindFunctions: (T: {
                             type t;
                           }) => Routable with type t = T.t;

module Router: {
  include Routable;
  let make:
    (~caseSensitive: bool=?, ~mergeParams: bool=?, ~strict: bool=?, unit) => t;
  external asMiddleware: t => Middleware.t = "%identity";
};

module HttpServer: {
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

let router:
  (~caseSensitive: bool=?, ~mergeParams: bool=?, ~strict: bool=?, unit) =>
  Router.t;

module App: {
  include Routable;
  let useRouter: (t, Router.t) => unit;
  let useRouterOnPath: (t, ~path: string, Router.t) => unit;
  [@mel.module] external make: unit => t = "express";
  external asMiddleware: t => Middleware.t = "%identity";
  let listen:
    (
      t,
      ~port: int=?,
      ~hostname: string=?,
      ~onListen: Js.Nullable.t(Js.Exn.t) => unit=?,
      unit
    ) =>
    HttpServer.t;
  [@mel.send] external disable: (t, ~name: string) => unit = "disable";
  [@mel.send] external set: (t, string, string) => unit = "set";
};

/** [express ()] creates an instance of the App class.
     Alias for [App.make ()] */
let express: unit => App.t;

module Static: {
  type options;
  type stat;
  let defaultOptions: unit => options;
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

  /** [make directory] creates a static middleware for [directory] */
  [@mel.module "express"]
  external make: (string, options) => t = "static";

  /** [asMiddleware static] casts [static] to a Middleware type */
  external asMiddleware: t => Middleware.t = "%identity";
};
